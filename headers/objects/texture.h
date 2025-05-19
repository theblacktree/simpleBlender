#ifndef TEXTURE_H
#define TEXTURE_H
#include <GL/glew.h>
#include <GL/gl.h>
#include <qopenglext.h>
#include <ostream>
#include <memory>

enum class TextureType {
    DIFFUSE_MAP,
    NORMAL_MAP,
    CUBE_MAP
};
class Texture {

public:
    // 构造函数：创建一个空纹理对象
    Texture(TextureType textureType, GLenum textureTarget = GL_TEXTURE_2D)
        : m_id(0), m_width(0), m_height(0), m_target(textureTarget), m_type(textureType) {}

    // 析构函数：释放纹理资源
    ~Texture() {
        release();
    }

    // 获取纹理类型
    inline TextureType getType() const { return m_type; }

    // 绑定纹理到指定单元
    void bind() const;

    // 释放纹理资源
    void release();

    // 设置纹理数据（从外部加载后设置）
    void setTextureData(GLuint textureID, int w, int h);

    // 获取纹理 ID
    GLuint& getId() { return m_id; }

    // 获取纹理尺寸
    int getWidth() const { return m_width; }
    int getHeight() const { return m_height; }

private:
    GLuint m_id;          // 纹理 ID
    int m_width, m_height;  // 纹理尺寸
    GLenum m_target;      // 纹理目标类型（如 GL_TEXTURE_2D 或 GL_TEXTURE_CUBE_MAP）
    TextureType m_type;   // 纹理类型

};

#endif // TEXTURE_H
