#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#include "texture.h"
#include <map>
#include <memory>
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <fstream>
//#include <GL/gl.h>   // 使用系统自带的 OpenGL 库时
//#include <qopenglext.h>
class TextureManager {
private:
    std::map<std::wstring, std::shared_ptr<Texture>> m_diffuseTextures;
    std::map<std::wstring, std::shared_ptr<Texture>> m_normalTextures;
    std::map<std::wstring, std::shared_ptr<Texture>> m_cubeTextures;
    std::unique_ptr<std::pair<std::wstring, GLuint >> m_hdrTexturePair;//hdr载入
    GLuint m_textureSkyBoxCubemap;//天空盒

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

    // 加载立方体贴图
    bool loadCubeMap(const std::wstring cubeMapFiles[6]);

    // 获取纹理
    std::shared_ptr<Texture> getTexture(const std::wstring &filePath) const;

    // 释放所有资源
    void releaseAll() {
        m_diffuseTextures.clear();
        m_normalTextures.clear();
    }
    void removeOneTexture(GLuint textureId);

    //hdr相关函数
    bool loadHDRMap(const std::wstring &filePath);
    void convertEquirectangularToCubemap(const cv::Mat& equirectangular, std::vector<cv::Mat>& cubemapFaces, int faceSize);
    cv::Vec3f latLongToCubeMap(float theta, float phi);
    void uploadCubemapToOpenGL(const std::vector<cv::Mat>& cubemapFaces, GLuint& textureID);
    GLuint getHDRTextureId();
};


#endif // TEXTUREMANAGER_H
