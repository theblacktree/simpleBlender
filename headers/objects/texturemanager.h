#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#include "texture.h"
#include <map>
#include <memory>
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <fstream>
//#include "mesh.h"
//#include <GL/gl.h>   // 使用系统自带的 OpenGL 库时
//#include <qopenglext.h>
namespace constTextureDefine
{
    const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
}
class TextureManager {
private:
    std::map<std::wstring, std::shared_ptr<Texture>> m_diffuseTextures;
    std::map<std::wstring, std::shared_ptr<Texture>> m_normalTextures;
    std::map<std::string, std::shared_ptr<Texture>> m_cubeTextures;
    std::unique_ptr<std::pair<std::wstring, GLuint >> m_hdrTexturePair;//hdr载入
    std::map<std::string, std::shared_ptr<Texture>> m_assimpTexturesLoaded;//这个是assimp库加载的2d纹理,键是路径
    GLuint m_depthCubemap;//光源的深度立方体贴图
    GLuint m_depthMapFBO;

    GLuint m_textureSkyBoxCubemap;//立方体贴图天空盒,从hdr中解析而来
    int m_resolution = 512;
    //由hdr提取立方体贴图使用，提取6个面
    GLuint m_captureFBO, m_captureRBO;

    TextureManager();
    ~TextureManager();
public:
    TextureManager( TextureManager& t) = delete;
    TextureManager& operator=( TextureManager& t) = delete;
    static TextureManager& GetInstance()
    {
        static TextureManager tf;
        return tf;
    }
    // 加载 2D 纹理
    bool load2DTexture(const std::wstring &filePath, TextureType type);

    // 加载默认的第一个立方体贴图，程序起来后创建
    bool loadCubeMap(std::string filePath);

    // 获取漫反射贴图和法线贴图纹理
    std::shared_ptr<Texture> getTexture(const std::wstring &filePath) const;
    //获取cubemap贴图纹理
    std::shared_ptr<Texture> getcubemapTexture(const std::string &filePath) const;
    //加载assimp纹理
    GLuint load2DAssimpTexture(const std::string &filePath, TextureType type);

    //加载深度立方体贴图，用于光源的阴影
    void initDotLightDepthCubeMap();
    //获取深度立方体贴图和深度帧缓冲
    GLuint getDepthCubeMap(){return m_depthCubemap;}
    GLuint getDepthMapFBO(){return m_depthMapFBO;}
    // 释放所有资源
    void releaseAll() {
        m_diffuseTextures.clear();
        m_normalTextures.clear();
    }
    void removeOneTexture(GLuint textureId);

    GLenum glCheckError_(const char *file, int line);

    //hdr相关函数
    bool loadHDRMap(const std::wstring &filePath);
    void convertEquirectangularToCubemap(const cv::Mat& equirectangular, std::vector<cv::Mat>& cubemapFaces, int faceSize);
    cv::Vec3f latLongToCubeMap(float theta, float phi);
    void uploadCubemapToOpenGL(const std::vector<cv::Mat>& cubemapFaces, GLuint& textureID);
    GLuint getHDRTextureId(){return m_hdrTexturePair->second;}
    //初始化相关的buffer,避免重复初始化
    void initTexBuffers();
    GLuint getFBO(){return m_captureFBO;}
    GLuint getRBO(){return m_captureRBO;}
    GLuint getSkyBoxCubeMap(){return m_textureSkyBoxCubemap;}
};


#endif // TEXTUREMANAGER_H
