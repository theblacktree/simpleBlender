#include "glwidget.h"
#include "converttoshaderlight.h"
#include "mainwindow.h"
using namespace std;
using namespace  glm;
GLWidget::GLWidget(MainWindow* mainWindow, QWidget *parent) : QOpenGLWidget(parent), m_mainWindow(mainWindow),allLights(MAX_LIGHTS, ShaderLight())
{
    setFocusPolicy(Qt::StrongFocus); // 允许接收键盘和鼠标事件
    //在这里创建一个默认的摄像机，这个摄像机在用户没创建的时候是默认使用的
    m_defaultCamera = new Camera();
    m_currentCamera = m_defaultCamera;
    connect(m_mainWindow, &MainWindow::sigRemove, this, &GLWidget::slotRemoveObjects);

    connect(m_mainWindow, &MainWindow::sigSetModel, this, &GLWidget::slotSetModelMatrix);

    connect(m_mainWindow, &MainWindow::sigColorSelected, this, &GLWidget::slotSetColor);

    connect(m_mainWindow, &MainWindow::sigSetMaterialValue, this, &GLWidget::slotSetMaterialValue);

    connect(m_mainWindow, &MainWindow::sigSwitchCamera, this, &GLWidget::slotSwitchCamera);

    connect(m_mainWindow, &MainWindow::sigSetCameraAttribute, this, &GLWidget::slotSetCameraAttribute);

    connect(m_mainWindow, &MainWindow::sigSetLightAttribute, this, &GLWidget::slotSetLightAttribute);

    connect(m_mainWindow, &MainWindow::sigSetObjectUVTexture, this, &GLWidget::slotSetObjecxtUVTexture);

    connect(m_mainWindow, &MainWindow::sigRemoveUVTextures, this, &GLWidget::slotRemoveObjectUVTexture);

    connect(m_mainWindow, &MainWindow::sigCreateHDRMap, this, &GLWidget::slotCreateHDRMAP);
    m_cuskybox = new HDRObject;
    m_roomBox = new cube;

}

GLWidget::~GLWidget()
{
    // 删除所有物体
    for (auto& [key, value]:m_objects)
    {
        Object* obj = value;
        delete obj;
    }
    m_objects.clear();
    for (auto& [key, value]:m_lights)
    {
        Light* light = value;
        delete light;
    }
    m_lights.clear();
    //删除摄像机类
    for (auto& [key, value]:m_cameras)
    {
        Camera* cam = value;
        delete cam;
    }
    m_objects.clear();
}

void GLWidget::slotSetMaterialValue(const vector<string>&selectedVec, float metallic, float roughness, float ior)
{
    for (const string& str:selectedVec)
    {
        Object* obj = getObject(str);
        if (obj) {
            obj->setMaterialValue(metallic, roughness, ior);
        }
    }
    update();
}

void GLWidget::slotSetColor(const std::vector<string> &selectedVec, QColor& color)
{
    //根据选定的物体去更改它的颜色属性
    for (const string& str:selectedVec)
    {
        Object* obj = getObject(str);
        if (obj) {
            obj->setObjectColor(color);
        }
    }
    //根据选中的灯光去改变它的颜色属性
    for (const string& str:selectedVec)
    {
        Light* light = getLight(str);
        if (light) {
            light->setLightColor(color);
        }
    }
    update();
}

void GLWidget::slotSwitchCamera(const std::vector<std::string> &selectedVec)
{

    for (const string& str:selectedVec)
    {
        Camera* cam = getCamera(str);
        if (cam == nullptr)
            return ;
        //在这里切换到该摄像机视角，然后绘制出来,具体就是将透射矩阵等信息设置
        m_currentCamera = cam;
    }
    update();
}

void GLWidget::slotSetCameraAttribute(const std::vector<std::string>&strSelectedCamera, int cameratype, float fov, float yaw, float pitch, float clipNear, float clipFar, glm::vec3 &position)
{
    for (const string& str:strSelectedCamera)
    {
        Camera* cam = getCamera(str);
        if (cam == nullptr)
            return ;
        //在这里将参数的信息设置到选中的摄像机中，并更新当前摄像机的信息
        cam->setCameraAttribute(cameratype, fov, yaw, pitch, clipNear, clipFar, position);
        m_currentCamera = cam;
    }
    update();
}

void GLWidget::addObject(Object* object, QString objectName)
{
    if (!object) {
        qWarning() << "Null object passed to addObject.";
        return;
    }

    // 添加对象到映射中
    string s = objectName.toStdString();
    m_objects[s] = object;

    // 确保在 OpenGL 上下文准备好的情况下调用 initialize
    QOpenGLContext* currentContext = QOpenGLContext::currentContext();
    if (currentContext && currentContext->isValid())
    {
        makeCurrent();
        //这条语句输出非常详细，列出了当前的上下文信息
        //qDebug() << "After makeCurrent, current context:" << QOpenGLContext::currentContext();

        // 确认 OpenGL 函数指针已初始化
        if (!context()->functions())
        {
            qWarning() << "OpenGL functions are not initialized.";
        }
        else
        {
            object->initialize();
        }

        doneCurrent(); // 完成后释放上下文
    }
    else
    {
        qWarning() << "Failed to initialize object: No valid OpenGL context.";
    }
    update();
}

void GLWidget::addLight(Light* light, int lightId)
{
    if (!light) {
        qWarning() << "Null light passed to addLight.";
        return;
    }

    // 添加对象到映射中
    string s = light->getLightTypeName().toStdString();
  //  pair p = make_pair(s +"_"+ std::to_string(lightId), light);
    m_lights[s +"_"+ std::to_string(lightId)] = light;
   // m_lights.emplace_back(p);
    //这里将所有灯光
    update();
}

void GLWidget::addCamera(Camera* camera, int cameraId)
{
    if (!camera) {
        qWarning() << "Null object passed to addObject.";
        return;
    }

    // 添加对象到映射中
    m_cameras["camera_" + to_string(cameraId)] = camera;

    update();
}
void GLWidget::slotRemoveObjects(const vector<std::string>&strRemove)
{
    if (strRemove.size() == 0)
        return ;
    for (auto& str:strRemove)
    {
        if (m_objects.count(str))
        {
            Object* obj = m_objects[str];
            m_objects.erase(str);
            delete obj;

        }
        for (auto it = m_lights.begin(); it != m_lights.end(); )
        {
            if ((*it).first == str)
            {
                Light* light = (*it).second;
               it = m_lights.erase(it);
                delete light;
            }
            else
            {
                ++it;
            }

        }
        if (m_cameras.count(str))
        {
            Camera* camera = m_cameras[str];
            m_cameras.erase(str);
            delete camera;

        }
    }
    if (m_objects.size() == 0)
    {
        m_mainWindow->resetObjectId();
    }
    if (m_lights.size() == 0)
    {
        m_mainWindow->resetLightId();
    }
    if (m_cameras.size() == 0)
    {
        m_mainWindow->resetCameraId();
        m_currentCamera = m_defaultCamera;
    }
    update();

}

Object* GLWidget::getObject(const string &str)
{
    if (m_objects.count(str))
        return m_objects[str];
    return nullptr;
}

Light* GLWidget::getLight(const std::string& str)
{
    for (auto& pair : m_lights)
    {
        if (pair.first == str)
            return pair.second;
    }
    return nullptr;
}

Camera* GLWidget::getCamera(const std::string& str)
{
    if (m_cameras.count(str))
        return m_cameras[str];
    return nullptr;
}

cv::Mat GLWidget::captureFrame()
{
    int width = this->width();
    int height = this->height();

    // 创建缓冲区存储像素数据
    std::vector<unsigned char> pixels(width * height * 3);

    // 从帧缓冲区读取 RGB 数据
    glPixelStorei(GL_PACK_ALIGNMENT, 1);
   // glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
    glReadPixels(0, height / 2, width, height , GL_RGB, GL_UNSIGNED_BYTE, pixels.data());
    // 创建 OpenCV Mat 对象
    cv::Mat image(height, width, CV_8UC3, pixels.data());

    // OpenGL 图像是上下颠倒的，需要翻转
    cv::flip(image, image, 0);

    return image.clone(); // 返回深拷贝，避免临时变量销毁
}

void GLWidget::addShader(QString &shaderPath, /*QString &fragmentPath,*/ GLuint &Shader, unsigned int shaderType/*, GLuint &fragmentShader*/)
{
    //着色器
    // 加载顶点着色器文件
    QFile vertfile(shaderPath);
    if (!vertfile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        throw std::runtime_error("Failed to open shader file: " + shaderPath.toStdString());
    }
    QTextStream vertin(&vertfile);
    std::string vertexSource = vertin.readAll().toStdString();
    vertfile.close();

    // // 加载片段着色器文件
    // QFile fragfile(fragmentPath);
    // if (!fragfile.open(QIODevice::ReadOnly | QIODevice::Text)) {
    //     throw std::runtime_error("Failed to open shader file: " + fragmentPath.toStdString());
    // }
    // QTextStream fragin(&fragfile);
    // std::string fragmentSource = fragin.readAll().toStdString();
    // fragfile.close();

    // 创建并编译顶点着色器
    Shader = glCreateShader(shaderType);//GL_VERTEX_SHADER
    const char* vertexSrc = vertexSource.c_str();
    glShaderSource(Shader, 1, &vertexSrc, nullptr);
    glCompileShader(Shader);

    // 检查顶点着色器编译错误
    int success;
    glGetShaderiv(Shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(Shader, 512, nullptr, infoLog);
        throw std::runtime_error("shader compilation failed: " + std::string(infoLog));
    }

    // // 创建并编译片段着色器
    // fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    // const char* fragmentSrc = fragmentSource.c_str();
    // glShaderSource(fragmentShader, 1, &fragmentSrc, nullptr);
    // glCompileShader(fragmentShader);

    // // 检查片段着色器编译错误
    // glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    // if (!success) {
    //     char infoLog[512];
    //     glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
    //     throw std::runtime_error("Fragment shader compilation failed: " + std::string(infoLog));
    // }
}

void GLWidget::renameOneItem(const std::string oldStr, const std::string newStr)
{
    if (m_objects.count(oldStr))
    {
        m_objects[newStr] = m_objects[oldStr];
        m_objects.erase(oldStr);
    }
    else if (m_lights.count(oldStr))
    {
        m_lights[newStr] = m_lights[oldStr];
        m_lights.erase(oldStr);
    }
    else if (m_cameras.count(oldStr))
    {
        m_cameras[newStr] = m_cameras[oldStr];
        m_cameras.erase(oldStr);
    }
    else
    {

    }
}

void GLWidget::addCubeMap(std::string currentItemText, std::string filePath, TextureType textureType)
{
    if (textureType == TextureType::CUBE_MAP)
    {
        TextureManager::GetInstance().loadCubeMap(filePath);
        m_objects[currentItemText]->setCubeTexture(TextureManager::GetInstance().getcubemapTexture(filePath)->getId());
    }

}
//加载assimp模型
void GLWidget::loadAssimpModel(std::string path)
{
    Assimp::Importer import;
    const aiScene *scene = import.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);
//debug加载模型有些慢
    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        cout << "ERROR::ASSIMP::" << import.GetErrorString() << endl;
        return;
    }
    m_assimpdirectory = path.substr(0, path.find_last_of('/'));

    processAssimpNode(scene->mRootNode, scene);
}

void GLWidget::processAssimpNode(aiNode *node, const aiScene *scene)
{
    // 处理节点所有的网格（如果有的话）
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        m_AssimpMeshs.insert( processAssimpMesh(mesh, scene));
        update();//添加一个物体更新一次
    }
    // 接下来对它的子节点重复这一过程
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processAssimpNode(node->mChildren[i], scene);
    }
}
//调用这个函数将模型中的aimesh数据加载到我们自己的Mesh对象中
Mesh* GLWidget::processAssimpMesh(aiMesh *mesh, const aiScene *scene)
{
    vector<Mesh::Vertex> vertices;
    vector<unsigned int> indices;
    vector<Mesh::AssimpTexture> textures;
    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Object::Vertex vertex;// = {0.0f};
        //memset(&vertex, 0x00, sizeof(vertex));
        // // 处理顶点位置、法线和纹理坐标
         vertex.x = mesh->mVertices[i].x;
         vertex.y = mesh->mVertices[i].y;
         vertex.z = mesh->mVertices[i].z;

         //颜色信息,这是我自己加的
         if (mesh->mColors[0] != nullptr)
         {
             vertex.r = mesh->mColors[0][i].r;
             vertex.g = mesh->mColors[0][i].g;
             vertex.b = mesh->mColors[0][i].b;
             vertex.a = mesh->mColors[0][i].a;
         }
         else
         {
             vertex.r = 0.3;
             vertex.g = 0.5;
             vertex.b = 0.4;
             vertex.a = 1.0;
         }

         //法线坐标
          if (mesh->HasNormals())
        {
            vertex.nx = mesh->mNormals[i].x;
            vertex.ny = mesh->mNormals[i].y;
            vertex.nz = mesh->mNormals[i].z;
        }
        else
        {
              vertex.nx = 0;
              vertex.ny = 0;
              vertex.nz = 0;
        }

         //纹理uv坐标
         if(mesh->mTextureCoords[0]) // 网格是否有纹理坐标？
         {//uv坐标的第三个分量暂时不考虑
             vertex.u = mesh->mTextureCoords[0][i].x;
             vertex.v = mesh->mTextureCoords[0][i].y;
         }
         else
         {
             vertex.u = 0;
             vertex.v = 0;
         }
         //切线和副切线
        if (mesh->mTangents != nullptr)
        {
             vertex.tangentX = mesh->mTangents[i].x;
             vertex.tangentY = mesh->mTangents[i].y;
             vertex.tangentZ = mesh->mTangents[i].z;

             vertex.bitangentX = mesh->mBitangents[i].x;
             vertex.bitangentY = mesh->mBitangents[i].y;
             vertex.bitangentZ = mesh->mBitangents[i].z;
        }
        else
        {
            vertex.tangentX = 0;
            vertex.tangentY = 0;
            vertex.tangentZ = 0;

            vertex.bitangentX = 0;
            vertex.bitangentY = 0;
            vertex.bitangentZ = 0;
        }

        vertices.push_back(vertex);
    }
    // 处理索引
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
        // 处理材质
    if(mesh->mMaterialIndex >= 0)
    {
        aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
        //漫反射贴图
        vector<Mesh::AssimpTexture> diffuseMaps = loadMaterialTextures(material,
                                                           aiTextureType_DIFFUSE, TextureType::DIFFUSE_MAP);
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        //这边可能需要更多的贴图类型
        //高光贴图
        vector<Mesh::AssimpTexture> specularMaps = loadMaterialTextures(material,
                                                            aiTextureType_SPECULAR, TextureType::SPECULAR_MAP);
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        //法线贴图
        vector<Mesh::AssimpTexture> normalMaps = loadMaterialTextures(material, \
                                                             aiTextureType_NORMALS, TextureType::NORMAL_MAP);
         textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
        //高度贴图
        vector<Mesh::AssimpTexture> heightMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, TextureType::HEIGHT_MAP);
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
        // 环境光贴图
        vector<Mesh::AssimpTexture> ambientMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, TextureType::AMBIENT_MAP);
        textures.insert(textures.end(), ambientMaps.begin(), ambientMaps.end());
        //粗糙度贴图
        vector<Mesh::AssimpTexture> roughnessMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE_ROUGHNESS, TextureType::ROUGHNESS_MAP);
        textures.insert(textures.end(), roughnessMaps.begin(), roughnessMaps.end());
        //AO贴图
        vector<Mesh::AssimpTexture> AOMaps = loadMaterialTextures(material, aiTextureType_AMBIENT_OCCLUSION, TextureType::AO_MAP);
        textures.insert(textures.end(), AOMaps.begin(), AOMaps.end());
    }
    Mesh* meshInstance = new Mesh(vertices, indices, textures);
    meshInstance->initialize();
    return meshInstance;
}
vector<Mesh::AssimpTexture> GLWidget:: loadMaterialTextures(aiMaterial *mat, aiTextureType type, TextureType typeEnumId)
{
    vector<Mesh::AssimpTexture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;

        Mesh::AssimpTexture texture;
        string filepath = m_assimpdirectory + "/"+string(str.C_Str());
        texture.id = TextureManager::GetInstance().load2DAssimpTexture(filepath, typeEnumId);
        texture.type = typeEnumId;
        texture.path = str.C_Str();
        cout<<str.C_Str()<<static_cast<int>(typeEnumId)<<endl;
        textures.push_back(texture);
    }
    return textures;
}
int GLWidget::getObjectCount() const
{
    return m_objects.size();
}

void GLWidget::slotSetModelMatrix(const vector<string> &strvec, glm::vec3& translationXYZ, glm::vec3& rotationXYZ, glm::vec3& scaleXYZ)
{
    for (const string& str:strvec)
    {
        Object* obj = getObject(str);
        if (obj) {
            obj->setModelMatrix(translationXYZ, rotationXYZ, scaleXYZ);
        }
    }
    update();
}

void GLWidget::slotSetLightAttribute(const std::vector<std::string> &strSelectedLight, ShaderLight &sl)
{
    for (auto str:strSelectedLight)
    {
        if (m_lights.count(str))
        {
            Light* light = m_lights[str];
            light->setAllAttribute(sl);
        }
    }
    update();
}

void GLWidget::slotSetObjecxtUVTexture(const std::string &selectedString, std::wstring &textureFilePath, TextureType textureType)
{
    //在这里设置某物体的纹理，该纹理传递给物体管理，并且启用该纹理渲染该物体

    TextureManager::GetInstance().load2DTexture(textureFilePath,textureType);
    if (!m_objects.count(selectedString))
        return ;
    if (textureType == TextureType::DIFFUSE_MAP)
        m_objects[selectedString]->setDiffuseTexture(TextureManager::GetInstance().getTexture(textureFilePath)->getId());
    else if (textureType == TextureType::NORMAL_MAP)
        m_objects[selectedString]->setNormalTexture(TextureManager::GetInstance().getTexture(textureFilePath)->getId()); 
    else
    {

    }
}

void GLWidget::slotRemoveObjectUVTexture(const std::string &selectedString, TextureType textureType)
{
    if (!m_objects.count(selectedString))
        return ;
    m_objects[selectedString]->removeUVTexture(textureType);
    update();
}

void GLWidget::slotCreateHDRMAP(std::wstring &HDRFilePath)
{

    TextureManager::GetInstance().loadHDRMap(HDRFilePath);
    update();
}

void GLWidget::initializeGL()
{
    // if (!glewInit()) {
    //     std::cerr << "Failed to initialize GLFW" << std::endl;
    //  }
    initializeOpenGLFunctions();
   // glClearColor(0.3f, 0.5f, 0.7f, 1.0f); // 设置背景颜色蓝色
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);
    glEnable(GL_MULTISAMPLE);
   // glDepthMask(GL_FALSE);
    glCheckError_("glwidget",604);
    m_cuskybox->initialize();
    m_roomBox->initialize();
    for (auto&[key,obj] : m_objects) {
        obj->initialize();
    }
    //初始化模型数据
    for (auto& mesh : m_AssimpMeshs) {
        mesh->initialize();
    }
glCheckError_("glwidget",614);

    // 创建并链接着色器程序
    //渲染物体的pbr着色器
    m_objectProgram = glCreateProgram();
    GLuint objectVertexShader;
    GLuint objectFragmentShader;
    GLuint lightGeomitryShader;//用于计算阴影的几何着色器
    addShader(m_objectVertexPath, objectVertexShader, GL_VERTEX_SHADER);
    addShader(m_objectFragmentPath, objectFragmentShader, GL_FRAGMENT_SHADER);
    glAttachShader(m_objectProgram, objectVertexShader);
    glAttachShader(m_objectProgram, objectFragmentShader);
    glLinkProgram(m_objectProgram);
    glCheckError_("glwidget",625);
    //渲染物体的天空盒着色器
    m_skyboxProgram = glCreateProgram();
    GLuint skyboxVertexShader;
    GLuint skyboxFragmentShader;
    addShader(m_skyboxVertexPath, skyboxVertexShader, GL_VERTEX_SHADER);
    addShader(m_skyboxFragmentPath,  skyboxFragmentShader, GL_FRAGMENT_SHADER);
    glAttachShader(m_skyboxProgram, skyboxVertexShader);
    glAttachShader(m_skyboxProgram, skyboxFragmentShader);
    glLinkProgram(m_skyboxProgram);
    glCheckError_("glwidget",634);
    //将hdr转换为立方体贴图的着色器
    m_hdrtocubemapProgram = glCreateProgram();
    GLuint hdrtocubemapVertexShader;
    GLuint hdrtocubemapFragmentShader;
    addShader(m_hdrtocubemapVertexPath,  hdrtocubemapVertexShader, GL_VERTEX_SHADER);
    addShader(m_hdrtocubemapFragmentPath,hdrtocubemapFragmentShader, GL_FRAGMENT_SHADER);
    glAttachShader(m_hdrtocubemapProgram, hdrtocubemapVertexShader);
    glAttachShader(m_hdrtocubemapProgram, hdrtocubemapFragmentShader);
    glLinkProgram(m_hdrtocubemapProgram);
    glCheckError_("glwidget",643);
    //assimpbli-phone着色器
    m_assimploadProgram = glCreateProgram();
    GLuint assimploadVertexShader;
    GLuint assimploadFragmentShader;
    addShader(m_assimploadVertexPath, assimploadVertexShader, GL_VERTEX_SHADER);
    addShader( m_assimploadFragmentPath, assimploadFragmentShader, GL_FRAGMENT_SHADER);
    glAttachShader(m_assimploadProgram, assimploadVertexShader);
    glAttachShader(m_assimploadProgram, assimploadFragmentShader);
    glLinkProgram(m_assimploadProgram);
    glCheckError_("glwidget",652);

    //创建阴影映射所需要的几何着色器


    // 检查程序链接错误
    int success;
    glGetProgramiv(m_objectProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(m_objectProgram, 512, nullptr, infoLog);
        throw std::runtime_error("Shader program linking failed: " + std::string(infoLog));
    }

    int successskybox;
    glGetProgramiv(m_skyboxProgram, GL_LINK_STATUS, &successskybox);
    if (!successskybox) {
        char infoLog[512];
        glGetProgramInfoLog(m_skyboxProgram, 512, nullptr, infoLog);
        throw std::runtime_error("Shader program linking failed: " + std::string(infoLog));
    }
    glCheckError_("glwidget",671);
  //  glUseProgram(m_ubo);
    glUseProgram(m_objectProgram);

    glGenBuffers(1, &m_ubo);
    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        qCritical() << "1:" << error;
    }
    //在这里初始化万向点阴影映射所需要的立方体贴图和深度帧缓冲
    TextureManager::GetInstance().initDotLightDepthCubeMap();
    // 分配内存并上传初始数据
     // 初始化光源数组
    for (int i = 0; i < MAX_LIGHTS; ++i) {
        initShaderLight(allLights[i]);
    }
    // 在链接着色器程序后添加：
    cout<<"m_ubo在绑定前的值为："<<m_ubo<<endl;
    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    cout<<"m_ubo在绑定后的值为："<<m_ubo<<endl;
    GLenum error1 = glGetError();
    if (error1 != GL_NO_ERROR) {
        qCritical() << "2:" << error1;
    }
    glBufferData(GL_UNIFORM_BUFFER, sizeof(ShaderLight) * MAX_LIGHTS, allLights.data(), GL_DYNAMIC_DRAW);
    GLenum error2 = glGetError();
    if (error2 != GL_NO_ERROR) {
        qCritical() << "3:" << error2;
    }


    GLint boundUBO1;
    glGetIntegerv(GL_UNIFORM_BUFFER_BINDING, &boundUBO1);
    std::cout << "init UBO Binding: " << boundUBO1 << std::endl;
    // 绑定到绑定点 0
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_ubo);
    GLenum error3 = glGetError();
    if (error3 != GL_NO_ERROR) {
        qCritical() << "4:" << error3;
    }
    GLint boundUBO2;
    glGetIntegerv(GL_UNIFORM_BUFFER_BINDING, &boundUBO2);
    std::cout << "Currentinit UBO Binding: " << boundUBO2 << std::endl;
    GLuint blockIndex = glGetUniformBlockIndex(m_objectProgram, "LightBlock");
    if (blockIndex != GL_INVALID_INDEX) {
        glUniformBlockBinding(m_objectProgram, blockIndex, 0); // 绑定到绑定点 0
        GLint bindingPoint;
        glGetActiveUniformBlockiv(m_objectProgram, blockIndex, GL_UNIFORM_BLOCK_BINDING, &bindingPoint);
        std::cout << "LightBlock Binding Point: " << bindingPoint << std::endl;
    } else {
        qCritical() << "LightBlock not found in shader!";
    }
    GLint boundUBO;
    glGetIntegerv(GL_UNIFORM_BUFFER_BINDING, &boundUBO);
    std::cout << "Currentinit UBO Binding: " << boundUBO << std::endl;
    // 解绑缓冲区
   glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glUseProgram(0);
   glCheckError_("glwidget",727);
    //重复绑定ubo到assimpbliphone着色器
   glUseProgram(m_assimploadProgram);

   glGenBuffers(1, &m_ubo);
   // 分配内存并上传初始数据
   // 初始化光源数组
   for (int i = 0; i < MAX_LIGHTS; ++i) {
       initShaderLight(allLights[i]);
   }
   glCheckError_("glwidget",737);
   // 在链接着色器程序后添加：
   cout<<"m_ubo在绑定前的值为："<<m_ubo<<endl;
   glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
   cout<<"m_ubo在绑定后的值为："<<m_ubo<<endl;
   glBufferData(GL_UNIFORM_BUFFER, sizeof(ShaderLight) * MAX_LIGHTS, allLights.data(), GL_DYNAMIC_DRAW);

   // 绑定到绑定点 0
   glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_ubo);

   GLuint blockIndex1 = glGetUniformBlockIndex(m_assimploadProgram, "LightBlock");
   if (blockIndex1 != GL_INVALID_INDEX) {
       glUniformBlockBinding(m_assimploadProgram, blockIndex1, 0); // 绑定到绑定点 0
       GLint bindingPoint;
       glGetActiveUniformBlockiv(m_assimploadProgram, blockIndex1, GL_UNIFORM_BLOCK_BINDING, &bindingPoint);
       std::cout << "LightBlock Binding Point: " << bindingPoint << std::endl;
   } else {
       qCritical() << "LightBlock not found in shader!";
   }
   glCheckError_("glwidget",756);
   // 解绑缓冲区
   glBindBuffer(GL_UNIFORM_BUFFER, 0);

    // 删除已链接的着色器对象
   glDeleteShader(objectVertexShader);
   glDeleteShader(objectFragmentShader);
   glDeleteShader(skyboxVertexShader);
   glDeleteShader(skyboxFragmentShader);
   glDeleteShader(hdrtocubemapVertexShader);
   glDeleteShader(hdrtocubemapFragmentShader);
   glDeleteShader(assimploadVertexShader);
   glDeleteShader(assimploadFragmentShader);

   glUseProgram(0);
    glCheckError_("glwidget",771);
}

void GLWidget::paintGL()
{
    // if (TextureManager::GetInstance().getHDRTextureId() == 0)
       //drawPBR();
   // else
   //      drawPBRIBL();
        drawAssimpPhong();

}

void GLWidget::resizeGL(int w, int h)
{
   // glViewport(0, 0, w, h);
 /*   for (auto&[key,obj] : m_objects) {
        makeCurrent();
        obj->resize(w, h);
        doneCurrent();
    }
*/
    glViewport(0, 0, w, h);
  //  m_projection = identity<mat4>();
  //  m_projection = perspective(radians(45.0f),static_cast<float>(w)/h, 0.1f, 100.0f);
    float aspect = static_cast<float>(w) / static_cast<float>(h);
    m_currentCamera->setAspectRatio(aspect); // 45.0f 是视角角度

    //glMatrixMode(GL_MODELVIEW);
   // glLoadIdentity();
}

void GLWidget::drawPBR()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glCheckError_("glwidget",806);
    glUseProgram(m_hdrtocubemapProgram);
    //上传HDR贴图
    GLuint HDRTextureId = TextureManager::GetInstance().getHDRTextureId();
    GLint useHDRMapLoc = glGetUniformLocation(m_hdrtocubemapProgram, "isUseHDRMap");
    glUniform1i(useHDRMapLoc, HDRTextureId ? 1 : 0);
    glCheckError_("glwidget",812);
    // bind Texture
    // Activate and bind the diffuse texture
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, HDRTextureId);

    glUniform1i(glGetUniformLocation(m_hdrtocubemapProgram, "equirectangularMap"), 3); // GL_TEXTURE2
    glUseProgram(0);
    glCheckError_("glwidget",820);
    //处理灯光部分，设置ubo
    // 计算总光源数量
    int totalLights = m_lights.size();

    size_t index = 0;

    //先将m_lights所有数据转换为ShaderLight类型，调用模板函数

    glUseProgram(m_objectProgram);
    glCheckError_("glwidget",830);
    // 绑定 UBO
    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    GLenum error5 = glGetError();
    if (error5 != GL_NO_ERROR) {
        qCritical() << "5:" << error5;
    }
    // GLint beforeBinding;
    // glGetIntegerv(GL_UNIFORM_BUFFER_BINDING, &beforeBinding);
    // std::cout << "Before binding UBO: " << beforeBinding << std::endl;

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_ubo); // 强制绑定到 0
    glCheckError_("glwidget",833);
    // GLint afterBinding;
    // glGetIntegerv(GL_UNIFORM_BUFFER_BINDING, &afterBinding);
    // std::cout << "After binding UBO: " << afterBinding << std::endl;
    glCheckError_("glwidget",846);
    GLint numLightsLoc = glGetUniformLocation(m_objectProgram, "numLights");
    glUniform1i(numLightsLoc, totalLights);
    GLenum error6 = glGetError();
    if (error6 != GL_NO_ERROR) {
        qCritical() << "6:" << error6;
    }
    glCheckError_("glwidget",853);
    // 收集所有光源数据
    for (const auto& pair : m_lights)
    {
        ShaderLight sl;
        Light* light = pair.second;
        switch (light->getLightType())
        {
        case Type::Point:
        {//点光源
            sl = convertToShaderLight(*static_cast<PointLight*>(light));
            break;
        }
        case Type::Directional:
        {//方向光源
            sl = convertToShaderLight(*static_cast<DirectionalLight*>(light));
            break;
        }
        case Type::Spot:
        {//聚光灯
            sl = convertToShaderLight(*static_cast<SpotLight*>(light));
            break;
        }
        default:
        {
            cout<<"未知光源类型"<<endl;
            break;
        }
        }
        allLights[index] = sl;
        ++index;
    }
    glCheckError_("glwidget",885);
    // cout<<"灯光数量是："<<totalLights<<endl;

    // 更新 UBO 数据
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ShaderLight) * allLights.size(), allLights.data());
    GLenum error7 = glGetError();
    if (error7 != GL_NO_ERROR) {
        qCritical() << "7:" << error7;
    }
    // GLint boundUBO;
    // glGetIntegerv(GL_UNIFORM_BUFFER_BINDING, &boundUBO);
    // std::cout << "Currentpaint UBO Binding: " << boundUBO << std::endl;
    // 解绑 UBO
    glBindBuffer(GL_UNIFORM_BUFFER,0);
    glCheckError_("glwidget",899);

    //在这里处理阴影部分。
    // if (m_lights.size() >= 1)
    // {
    //     glBindFramebuffer(GL_FRAMEBUFFER, TextureManager::GetInstance().getDepthMapFBO());
    //     glViewport(0, 0, constTextureDefine::SHADOW_WIDTH, constTextureDefine::SHADOW_HEIGHT);
    //     glClear(GL_DEPTH_BUFFER_BIT);

    //     glUseProgram(m_objectProgram);
    //     // 不需要切换视图或重复绘制
    //     //renderScene(); // 只需绘制一次，几何着色器自动处理六个面
    //     // 在C++端将六个视图+投影矩阵传入着色器
    //     GLfloat aspect = (GLfloat)constTextureDefine::SHADOW_WIDTH/(GLfloat)constTextureDefine::SHADOW_HEIGHT;
    //     glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, 1.0f, 25.0f);

    //     std::vector<glm::mat4> shadowTransforms;
    //     glm::vec3 lightPos = glm::vec3(allLights[0].position.x, allLights[0].position.y, allLights[0].position.z);
    //     shadowTransforms.push_back(shadowProj *
    //                                glm::lookAt(lightPos, lightPos + glm::vec3(1.0,0.0,0.0), glm::vec3(0.0,-1.0,0.0)));
    //     shadowTransforms.push_back(shadowProj *
    //                                glm::lookAt(lightPos, lightPos + glm::vec3(-1.0,0.0,0.0), glm::vec3(0.0,-1.0,0.0)));
    //     shadowTransforms.push_back(shadowProj *
    //                                glm::lookAt(lightPos, lightPos + glm::vec3(0.0,1.0,0.0), glm::vec3(0.0,0.0,1.0)));
    //     shadowTransforms.push_back(shadowProj *
    //                                glm::lookAt(lightPos, lightPos + glm::vec3(0.0,-1.0,0.0), glm::vec3(0.0,0.0,-1.0)));
    //     shadowTransforms.push_back(shadowProj *
    //                                glm::lookAt(lightPos, lightPos + glm::vec3(0.0,0.0,1.0), glm::vec3(0.0,-1.0,0.0)));
    //     shadowTransforms.push_back(shadowProj *
    //                                glm::lookAt(lightPos, lightPos + glm::vec3(0.0,0.0,-1.0), glm::vec3(0.0,-1.0,0.0)));

    //     GLint lightSpaceMatrixLoc = glGetUniformLocation(m_objectProgram, "shadowMatrices");
    //     // shadowTransforms 是包含6个mat4的数组
    //     glUniformMatrix4fv(lightSpaceMatrixLoc, 6, GL_FALSE, glm::value_ptr(shadowTransforms[0]));
    //     glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // }








    //处理摄像机，上传当前摄像机的视图矩阵和透射矩阵和位置向量
    GLint viewLoc = glGetUniformLocation(m_objectProgram, "aview");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(m_currentCamera->getViewMatrix()));
    GLint projectionLoc = glGetUniformLocation(m_objectProgram, "aprojection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(m_currentCamera->getProjectionMatrix()));
    GLint camerapositionLoc = glGetUniformLocation(m_objectProgram, "acameraPosition");
    glUniform3fv(camerapositionLoc, 1, glm::value_ptr(m_currentCamera->getCameraPosition()));
    glCheckError_("glwidget",907);
    // 遍历所有物体并调用其绘制函数
    if (TextureManager::GetInstance().getHDRTextureId() != 0)
    {
        glCheckError_("glwidget",911);
        glUseProgram(m_hdrtocubemapProgram);
        GLint viewskyLoc = glGetUniformLocation(m_hdrtocubemapProgram, "view");
        glUniformMatrix4fv(viewskyLoc, 1, GL_FALSE, glm::value_ptr(m_currentCamera->getViewMatrix()));
        GLint projectionskyLoc = glGetUniformLocation(m_hdrtocubemapProgram, "projection");
        glUniformMatrix4fv(projectionskyLoc, 1, GL_FALSE, glm::value_ptr(m_currentCamera->getProjectionMatrix()));
        //glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(100.0f));
        // m_cuskybox->setModelMatrix(model);
        m_cuskybox->draw(m_hdrtocubemapProgram);
        glUseProgram(0);
        glCheckError_("glwidget",922);
    }
    // else if (TextureManager::GetInstance().getHDRTextureId() == 0)
    // {
    //     glCheckError_("glwidget",925);
    //     if (TextureManager::GetInstance().getcubemapTexture("room") == nullptr)
    //     {
    //         TextureManager::GetInstance().loadCubeMap();
    //     }
    //     glDepthMask(GL_FALSE);//确保天空盒不会更新深度缓冲区，从而不会遮挡其他物体，同时提升渲染效率
    //     //这时候在程序开始时设置一个有大小的天空盒
    //     glUseProgram(m_objectProgram);
    //     GLint viewskyLoc = glGetUniformLocation(m_objectProgram, "view");
    //     glUniformMatrix4fv(viewskyLoc, 1, GL_FALSE, glm::value_ptr(m_currentCamera->getViewMatrix()));
    //     GLint projectionskyLoc = glGetUniformLocation(m_objectProgram, "projection");
    //     glUniformMatrix4fv(projectionskyLoc, 1, GL_FALSE, glm::value_ptr(m_currentCamera->getProjectionMatrix()));
    //     m_roomBox->setCubeTexture(TextureManager::GetInstance().getcubemapTexture("room")->getId());
    //     glm::vec3 translate = glm::vec3(0, 0, 0);
    //     glm::vec3 rotate = glm::vec3(0, 0, 0);
    //     glm::vec3 scale = glm::vec3(100, 100, 100);
    //     m_roomBox->setModelMatrix(translate, rotate, scale);
    //   //  m_roomBox->draw(m_objectProgram);
    //     glUseProgram(0);
    //     glDepthMask(GL_TRUE);
    //     glCheckError_("glwidget",945);
    // }
    glCheckError_("glwidget",947);
    glUseProgram(m_objectProgram);
    for (auto&[key,obj] : m_objects) {
        obj->draw(m_objectProgram);
    }
    glCheckError_("glwidget",952);
    //渲染所有assimp导入的mesh物体
    for (auto& mesh : m_AssimpMeshs) {
        mesh->draw(m_objectProgram);
    }
    glCheckError_("glwidget",957);
    glUseProgram(0);
}

void GLWidget::drawPBRIBL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_CULL_FACE); // 或者 glCullFace(GL_FRONT);
    glCheckError_("glw",688);
    glUseProgram(m_hdrtocubemapProgram);
    //上传HDR贴图
    GLuint HDRTextureId = TextureManager::GetInstance().getHDRTextureId();
    GLint useHDRMapLoc = glGetUniformLocation(m_hdrtocubemapProgram, "isUseHDRMap");
    glUniform1i(useHDRMapLoc, HDRTextureId != 0 ? 1 : 0);
    glCheckError_("glw",694);
    // bind Texture
    // Activate and bind the hdr texture
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, HDRTextureId);

    glUniform1i(glGetUniformLocation(m_hdrtocubemapProgram, "equirectangularMap"), 3); // GL_TEXTURE2
    glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
    //处理摄像机，上传当前摄像机的视图矩阵和透射矩阵和位置向量
    glCheckError_("glw",703);
    // GLint viewskyLoc = glGetUniformLocation(m_hdrtocubemapProgram, "view");
    // glUniformMatrix4fv(viewskyLoc, 1, GL_FALSE, glm::value_ptr(captureViews[1]));
    GLint projectionskyLoc = glGetUniformLocation(m_hdrtocubemapProgram, "projection");
    glUniformMatrix4fv(projectionskyLoc, 1, GL_FALSE, glm::value_ptr(captureProjection));
    // glViewport(0, 0, 512, 512); // don't forget to configure the viewport to the capture dimensions.
    glBindFramebuffer(GL_FRAMEBUFFER, TextureManager::GetInstance().getFBO());
    for (unsigned int i = 0; i < 6; ++i)
    {
        glCheckError_("glw",712);
        GLint viewskyLoc = glGetUniformLocation(m_hdrtocubemapProgram, "view");
        glUniformMatrix4fv(viewskyLoc, 1, GL_FALSE, glm::value_ptr(captureViews[i]));
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, TextureManager::GetInstance().getSkyBoxCubeMap(), 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glCheckError_("glw",716);
        m_cuskybox->draw(m_hdrtocubemapProgram);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glCheckError_("glw",720);
    // glUseProgram(0);
    // 遍历所有物体并调用其绘制函数
    // m_cuskybox->draw(m_hdrtocubemapProgram);
    /*glUseProgram(m_objectProgram);
    for (auto&[key,obj] : m_objects) {
        obj->draw(m_objectProgram);
    }*/
    glUseProgram(0);
    glDepthFunc(GL_LEQUAL); // 让天空盒在最远
    glUseProgram(m_skyboxProgram);
    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_CUBE_MAP, TextureManager::GetInstance().getSkyBoxCubeMap());
    glUniform1i(glGetUniformLocation(m_skyboxProgram, "environmentMap"), 4);
    glCheckError_("glw",748);
     glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(10.0f));
     m_cuskybox->setModelMatrix(model);
    m_cuskybox->draw(m_skyboxProgram); // 绘制单位立方体作为天空盒
    glCheckError_("glw",750);

    glDepthFunc(GL_LESS); // 恢复默认深度测试
    glUseProgram(0);
}

void GLWidget::drawAssimpPhong()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //处理灯光部分，设置ubo
    // 计算总光源数量
    int totalLights = m_lights.size();

    size_t index = 0;

    //先将m_lights所有数据转换为ShaderLight类型，调用模板函数

    glUseProgram(m_assimploadProgram);
    // 绑定 UBO
    glBindBuffer(GL_UNIFORM_BUFFER, m_ubo);
    GLenum error5 = glGetError();
    if (error5 != GL_NO_ERROR) {
        qCritical() << "5:" << error5;
    }
    // GLint beforeBinding;
    // glGetIntegerv(GL_UNIFORM_BUFFER_BINDING, &beforeBinding);
    // std::cout << "Before binding UBO: " << beforeBinding << std::endl;

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_ubo); // 强制绑定到 0

    // GLint afterBinding;
    // glGetIntegerv(GL_UNIFORM_BUFFER_BINDING, &afterBinding);
    // std::cout << "After binding UBO: " << afterBinding << std::endl;

    GLint numLightsLoc = glGetUniformLocation(m_objectProgram, "numLights");
    glUniform1i(numLightsLoc, totalLights);
    // GLenum error6 = glGetError();//这里出错了，暂时注释掉
    // if (error6 != GL_NO_ERROR) {
    //     qCritical() << "6:" << error6;
    // }

    // 收集所有光源数据
    for (const auto& pair : m_lights)
    {
        ShaderLight sl;
        Light* light = pair.second;
        switch (light->getLightType())
        {
        case Type::Point:
        {//点光源
            sl = convertToShaderLight(*static_cast<PointLight*>(light));
            break;
        }
        case Type::Directional:
        {//方向光源
            sl = convertToShaderLight(*static_cast<DirectionalLight*>(light));
            break;
        }
        case Type::Spot:
        {//聚光灯
            sl = convertToShaderLight(*static_cast<SpotLight*>(light));
            break;
        }
        default:
        {
            cout<<"未知光源类型"<<endl;
            break;
        }
        }
        allLights[index] = sl;
        ++index;
    }
    // cout<<"灯光数量是："<<totalLights<<endl;

    // 更新 UBO 数据
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(ShaderLight) * allLights.size(), allLights.data());
    GLenum error7 = glGetError();
    if (error7 != GL_NO_ERROR) {
        qCritical() << "7:" << error7;
    }
    // GLint boundUBO;
    // glGetIntegerv(GL_UNIFORM_BUFFER_BINDING, &boundUBO);
    // std::cout << "Currentpaint UBO Binding: " << boundUBO << std::endl;
    // 解绑 UBO
    glBindBuffer(GL_UNIFORM_BUFFER,0);

    //处理摄像机，上传当前摄像机的视图矩阵和透射矩阵和位置向量
    GLint viewLoc = glGetUniformLocation(m_assimploadProgram, "view");
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(m_currentCamera->getViewMatrix()));
    GLint projectionLoc = glGetUniformLocation(m_assimploadProgram, "projection");
    glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(m_currentCamera->getProjectionMatrix()));
    GLint camerapositionLoc = glGetUniformLocation(m_assimploadProgram, "viewPos");
    glUniform3fv(camerapositionLoc, 1, glm::value_ptr(m_currentCamera->getCameraPosition()));
    // 遍历所有物体并调用其绘制函数
    glUseProgram(m_assimploadProgram);
    for (auto&[key,obj] : m_objects) {
        obj->draw(m_assimploadProgram);
    }
    //渲染所有assimp导入的mesh物体
    for (auto& mesh : m_AssimpMeshs) {
        mesh->draw(m_assimploadProgram);
    }
    glUseProgram(0);
}

GLenum GLWidget::glCheckError_(const char *file, int line)
{
    GLenum errorCode;
    while ((errorCode = glGetError()) != GL_NO_ERROR)
    {
        std::string error = "opengl error";
        switch (errorCode)
        {
        case GL_INVALID_ENUM:                  error = "INVALID_ENUM"; break;
        case GL_INVALID_VALUE:                 error = "INVALID_VALUE"; break;
        case GL_INVALID_OPERATION:             error = "INVALID_OPERATION"; break;
        case GL_STACK_OVERFLOW:                error = "STACK_OVERFLOW"; break;
        case GL_STACK_UNDERFLOW:               error = "STACK_UNDERFLOW"; break;
        case GL_OUT_OF_MEMORY:                 error = "OUT_OF_MEMORY"; break;
        case GL_INVALID_FRAMEBUFFER_OPERATION: error = "INVALID_FRAMEBUFFER_OPERATION"; break;
        default:error = "unkown opengl error";break;
        }
        std::cout << error << " | " << file << " (" << line << ")" << std::endl;
    }
    return errorCode;
}

