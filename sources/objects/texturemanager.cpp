#include "texturemanager.h"

TextureManager::TextureManager()
{
    m_hdrTexturePair = std::make_unique<std::pair<std::wstring, GLuint>>(L"", 0);

}
TextureManager::~TextureManager() {}

bool TextureManager::load2DTexture(const std::wstring& filePath, TextureType type)
{

    if (type == TextureType::DIFFUSE_MAP && m_diffuseTextures.find(filePath) != m_diffuseTextures.end())
    {
        std::wcout << "diffuseTexture already loaded: " << filePath << std::endl;
        return true;
    }
    else if (type == TextureType::NORMAL_MAP && m_normalTextures.find(filePath)!= m_normalTextures.end())
    {
        std::wcout << "normalTexture already loaded: " << filePath << std::endl;
        return true;
    }
    const std::filesystem::path fsPath( filePath );

    // Open the file as a binary stream
    std::ifstream file(fsPath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::wcerr << "Failed to open file: " << filePath << std::endl;
        return false;
    }

    // Get the file size
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    // Read the file into a buffer
    std::vector<char> buffer(size);
    if (!file.read(buffer.data(), size)) {
        std::wcerr << "Failed to read file: " << filePath << std::endl;
        return false;
    }

    // Decode the image from the buffer
    cv::Mat image = cv::imdecode(cv::Mat(buffer), cv::IMREAD_UNCHANGED);
    if (image.empty()) {
        std::wcerr << "Failed to decode image: " << filePath << std::endl;
    }
    // 转换为 RGBA 格式
    if (image.channels() == 3) {
        cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
    } else if (image.channels() == 4) {
        cv::cvtColor(image, image, cv::COLOR_BGRA2RGBA);
    }
    cv::flip(image, image, 0); // 0 表示绕 x 轴翻转
    // 创建新的 Texture 对象
    auto texture = std::make_shared<Texture>(type);

    glGenTextures(1, &(texture->getId()));
    glBindTexture(GL_TEXTURE_2D, texture->getId());

    // 设置纹理参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 将 OpenCV 数据上传到 GPU
    GLenum format = (image.channels() == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, image.cols, image.rows, 0, format, GL_UNSIGNED_BYTE, image.data);

    // 设置纹理数据
    texture->setTextureData(texture->getId(), image.cols, image.rows);

    // 保存纹理
    if (type == TextureType::DIFFUSE_MAP)
        m_diffuseTextures[filePath] = texture;
    else
        m_normalTextures[filePath] = texture;
    return true;
}

bool TextureManager::loadCubeMap(const std::wstring cubeMapFiles[]) {
    auto texture = std::make_shared<Texture>(TextureType::CUBE_MAP, GL_TEXTURE_CUBE_MAP);

    glGenTextures(1, &(texture->getId()));
    glBindTexture(GL_TEXTURE_CUBE_MAP, texture->getId());

    // 设置纹理参数
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // // 加载 6 张图片
    // for (unsigned int i = 0; i < 6; ++i) {
    //     cv::Mat image = cv::imread(cubeMapFiles[i], cv::IMREAD_UNCHANGED);
    //     if (image.empty()) {
    //         std::cerr << "Failed to load cube map face: " << cubeMapFiles[i] << std::endl;
    //         return false;
    //     }

    //     if (image.channels() == 3) {
    //         cv::cvtColor(image, image, cv::COLOR_BGR2RGB);
    //     } else if (image.channels() == 4) {
    //         cv::cvtColor(image, image, cv::COLOR_BGRA2RGBA);
    //     }

    //     GLenum format = (image.channels() == 4) ? GL_RGBA : GL_RGB;
    //     glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, image.cols, image.rows, 0, format, GL_UNSIGNED_BYTE, image.data);
    // }

    // // 保存纹理
    // m_diffuseTextures["cube_map"] = texture;

    return true;
}

std::shared_ptr<Texture> TextureManager::getTexture(const std::wstring& filePath) const {
    auto it = m_diffuseTextures.find(filePath);
    if (it != m_diffuseTextures.end()) {
        return it->second;
    }
    auto itnorm = m_normalTextures.find(filePath);
    if (itnorm != m_normalTextures.end())
    {
        return itnorm->second;
    }
    std::cout<<"get texture failed"<<std::endl;
    return nullptr;
}

void TextureManager::removeOneTexture(GLuint textureId)
{
    auto itdiffuse = m_diffuseTextures.begin();
    while (itdiffuse != m_diffuseTextures.end())
    {
        if (itdiffuse->second->getId() == textureId)
        {
            itdiffuse->second->release();
            itdiffuse->second.reset();
            m_diffuseTextures.erase(itdiffuse++);
            return ;
        }
        ++itdiffuse;
    }
    auto itnormal = m_normalTextures.begin();
    while (itnormal != m_normalTextures.end())
    {
        if (itnormal->second->getId() == textureId)
        {
            itnormal->second->release();
            m_normalTextures.erase(itnormal++);
            return ;
        }
        ++itnormal;
    }
}

bool TextureManager::loadHDRMap(const std::wstring &filePath)
{
    if (m_hdrTexturePair->first == filePath)
    {
        std::wcout << "diffuseTexture already loaded: " << filePath << std::endl;
        return true;
    }
    //首先把原来的textureId释放掉
    if (m_hdrTexturePair->second != 0)
    {
        glDeleteTextures(1, &m_hdrTexturePair->second);
        m_hdrTexturePair->second = 0;
    }
    const std::filesystem::path fsPath( filePath );

    // Open the file as a binary stream
    std::ifstream file(fsPath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::wcerr << "Failed to open file: " << filePath << std::endl;
        return false;
    }

    // Get the file size
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    // Read the file into a buffer
    std::vector<char> buffer(size);
    if (!file.read(buffer.data(), size)) {
        std::wcerr << "Failed to read file: " << filePath << std::endl;
        return false;
    }

    // Decode the hdrImage from the buffer
    cv::Mat hdrImage = cv::imdecode(cv::Mat(buffer), cv::IMREAD_UNCHANGED);
    if (hdrImage.empty()) {
        std::wcerr << "Failed to decode image: " << filePath << std::endl;
    }
    // 转换为 RGBA 格式
    if (hdrImage.channels() == 3) {
        cv::cvtColor(hdrImage, hdrImage, cv::COLOR_BGR2RGB);
    } else if (hdrImage.channels() == 4) {
        cv::cvtColor(hdrImage, hdrImage, cv::COLOR_BGRA2RGBA);
    }
    cv::flip(hdrImage, hdrImage, 0); // 0 表示绕 x 轴翻转

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    // 将HDR数据上传到GPU
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, hdrImage.cols, hdrImage.rows, 0,
                 GL_RGB, GL_FLOAT, hdrImage.ptr<float>());

    // 设置纹理参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    m_hdrTexturePair->first = filePath;
    m_hdrTexturePair->second = textureID;
   // glBindTexture(GL_TEXTURE_2D, 0); // 解绑纹理
    //在这里还得计算辐射度贴图 Irradiance Map、预过滤环境贴图 Prefiltered Environment Map 和 BRDF 查找表 BRDF LUT）
//首先创建立方体贴图以生成天空盒
    // std::vector<cv::Mat> cubemapFaces;
    // int faceSize = 1080; // 设定立方体贴图的大小
    // convertEquirectangularToCubemap(hdrImage, cubemapFaces, faceSize);
    // // 保存立方体贴图面
    // for(int i = 0; i < 6; ++i) {
    //     // 确保图像数据是连续的
    //     cv::Mat continuousImg;
    //     if(cubemapFaces[i].isContinuous()) {
    //         continuousImg = cubemapFaces[i];
    //     } else {
    //         continuousImg = cubemapFaces[i].clone();
    //     }

    //     // 如果需要，转换颜色空间从BGR到RGB
    //     cv::Mat imgForSave;
    //     cv::cvtColor(continuousImg, imgForSave, cv::COLOR_BGR2RGB);

    //     // 检查并调整图像的位深度为8位，如果必要的话进行缩放
    //     double minVal, maxVal;
    //     cv::minMaxLoc(imgForSave, &minVal, &maxVal); // 获取像素值范围
    //     if(imgForSave.depth() != CV_8U) { // 如果不是8位深度
    //         imgForSave.convertTo(imgForSave, CV_8U, 255.0 / maxVal); // 转换为8位，并根据最大值缩放
    //     }

    //     // 保存图像
    //     cv::imwrite("cubemap_face_" + std::to_string(i) + ".png", imgForSave);
    // }
    // uploadCubemapToOpenGL(cubemapFaces, m_textureSkyBoxCubemap);
//create cubemap
    GLuint cubemapTexture;
    glGenTextures(1, &cubemapTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

    int resolution = 512; // 设置立方体贴图分辨率
    for (unsigned int i = 0; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, resolution, resolution, 0, GL_RGB, GL_FLOAT, nullptr);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glGenerateMipmap(GL_TEXTURE_CUBE_MAP); // 可选：生成mipmap
//create FBO to render face by face
    GLuint captureFBO, captureRBO;
    glGenFramebuffers(1, &captureFBO);
    glGenRenderbuffers(1, &captureRBO);

    glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, resolution, resolution);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);
    return true;
}
// 将经纬度坐标转换为立方体面上的坐标
cv::Vec3f TextureManager::latLongToCubeMap(float theta, float phi) {
    // 计算三维空间中的方向向量
    float x = std::sin(phi) * std::cos(theta);
    float y = std::sin(phi) * std::sin(theta);
    float z = std::cos(phi);

    // 确定立方体面并调整坐标
    cv::Vec3f vec;
    if(std::abs(x) >= std::abs(y) && std::abs(x) >= std::abs(z)) { // Right or Left
        vec = cv::Vec3f(x >= 0 ? 1 : -1, y / std::abs(x), z / std::abs(x));
    } else if(std::abs(y) >= std::abs(x) && std::abs(y) >= std::abs(z)) { // Top or Bottom
        vec = cv::Vec3f(x / std::abs(y), y >= 0 ? 1 : -1, z / std::abs(y));
    } else { // Front or Back
        vec = cv::Vec3f(x / std::abs(z), y / std::abs(z), z >= 0 ? 1 : -1);
    }

    return vec;
}

void TextureManager::convertEquirectangularToCubemap(const cv::Mat& equirectangular, std::vector<cv::Mat>& cubemapFaces, int faceSize) {
    for(int i = 0; i < 6; ++i) {
        cubemapFaces.push_back(cv::Mat(faceSize, faceSize, CV_32FC3));
    }

    for(int y = 0; y < faceSize; ++y) {
        for(int x = 0; x < faceSize; ++x) {
            for(int face = 0; face < 6; ++face) {
                // 根据不同的面调整坐标
                float nx = (2.0f * (x + 0.5f) / faceSize - 1.0f);
                float ny = (2.0f * (y + 0.5f) / faceSize - 1.0f);

                cv::Vec3f vec = latLongToCubeMap(atan2(ny, (face == 0 || face == 4) ? 1 : (face == 1 || face == 5) ? -1 : nx),
                                                 acos((face == 2 || face == 4 || face == 5) ? -ny : ny));

                // 将方向向量转换为经纬度坐标
                float theta = std::atan2(vec[1], vec[0]);
                float phi = std::acos(std::clamp(vec[2], -1.0f, 1.0f));

                // 转换为球面图坐标
                int u = ((theta + M_PI) / (2 * M_PI)) * equirectangular.cols;
                int v = (phi / M_PI) * equirectangular.rows;

                u = std::max(0, std::min(equirectangular.cols - 1, u));
                v = std::max(0, std::min(equirectangular.rows - 1, v));

                cubemapFaces[face].at<cv::Vec3f>(y, x) = equirectangular.at<cv::Vec3f>(v, u);
            }
        }
    }
}

void TextureManager::uploadCubemapToOpenGL(const std::vector<cv::Mat>& cubemapFaces, GLuint& textureID) {
    // 生成纹理ID
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    // 定义立方体贴图的面
    GLenum targets[] = {
        GL_TEXTURE_CUBE_MAP_POSITIVE_X,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
        GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
    };

    for (int i = 0; i < 6; ++i) {
        // 获取当前面的数据指针
        void* data = cubemapFaces[i].isContinuous() ? static_cast<void*>(cubemapFaces[i].data)
                                                    : static_cast<void*>(cubemapFaces[i].clone().data);

        // 上传数据到相应的立方体贴图面
        glTexImage2D(targets[i], 0, GL_RGB16F, cubemapFaces[i].cols, cubemapFaces[i].rows, 0, GL_BGR, GL_FLOAT, data);

        // 如果图像不是连续存储的，释放克隆的数据
        if (!cubemapFaces[i].isContinuous()) {
            cv::Mat clone = cubemapFaces[i].clone();
            delete[] reinterpret_cast<uchar*>(const_cast<uchar*>(clone.data));
        }
    }

    // 设置纹理参数
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // 解绑纹理
   // glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}
GLuint TextureManager::getHDRTextureId()
{
    return m_hdrTexturePair->second;
}
