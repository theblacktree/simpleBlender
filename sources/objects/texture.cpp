#include "texture.h"
#include <iostream>
//#include <qopenglext.h>
using namespace std;
// 释放纹理资源
void Texture::bind() const
{
    if (m_id == 0)
    {//before you bind texture you need to setTextureData
        std::cerr << "Attempted to bind an invalid texture!" << std::endl;
        return;
    }
    // 根据纹理类型选择纹理单元
    GLuint unit = 0;
    switch (m_type)
    {
    case TextureType::DIFFUSE_MAP:
        unit = 0; // DIFUSE_MAP bind to GL_TEXTURE0
        break;
    case TextureType::NORMAL_MAP:
        unit = 1; // normal_map bind to GL_TEXTURE1
        break;
    case TextureType::CUBE_MAP:
        unit = 2; // cube_map bind to GL_TEXTURE2
        break;
    default:
        std::cerr << "Unknown texture type!" << std::endl;
        return;
    }
    // 激活指定的纹理单元
    glActiveTexture(GL_TEXTURE0 + unit);

    // 绑定纹理到当前激活的纹理单元
    glBindTexture(m_target, m_id);
}

void Texture::release() {
    if (m_id != 0) {
        glDeleteTextures(1, &m_id);
        m_id = 0;
    }
}

void Texture::setTextureData(GLuint textureID, int w, int h)
{
    m_id = textureID;
    m_width = w;
    m_height = h;
}
