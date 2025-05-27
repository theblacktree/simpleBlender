#ifndef CAMERA_H
#define CAMERA_H

#include "glm.hpp"
#include "gtc/matrix_transform.hpp"
#include "gtc/type_ptr.hpp" // 包含矩阵变换函数
#include <iostream>
#include <vector>
#include "toolFunc.h"

// 定义方向常量
enum CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera {
public:
    // 构造函数
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 4.0f),
           glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f),
           float yaw = -90.0f, float pitch = 0.0f);


    // 更新摄像机方向向量
    void updateCameraVectors();

    // 获取视图矩阵
    inline glm::mat4 getViewMatrix() const{return glm::lookAt(m_position, m_position + m_front, m_up);}

    // 获取投影矩阵
    glm::mat4 getProjectionMatrix() const ;
    //获取摄像机位置
    inline glm::vec3 getCameraPosition()const{return m_position;}
    //获取投影方式
    inline bool getIsUseOrthographic(){return m_isOrthographic;}
    //获取fov
    inline float getFOV(){return m_fov;}
    //设置宽高比
    inline void setAspectRatio(float aspect){m_aspectRatio = aspect;}
    //获取yaw
    inline float getYaw(){return m_yaw;}
    //获取pitch
    inline float getPitch(){return m_pitch;}
    //获取近远裁剪距离
    inline float getNearClip(){return m_nearClip;}
    inline float getFarClip(){return m_farClip;}



    // 处理键盘输入
    void processKeyboard(CameraMovement direction, float deltaTime);

    // 处理鼠标移动
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

    // 处理鼠标滚轮输入
    void processMouseScroll(float yoffset) {
        m_fov -= yoffset;
        if (m_fov < 1.0f)
            m_fov = 1.0f;
        if (m_fov > 45.0f)
            m_fov = 45.0f;
    }

    // 切换投影模式
    inline void toggleProjectionMode() {m_isOrthographic = !m_isOrthographic;}

    // 调试输出
    void debugPrint() const {
        std::cout << "Position: (" << m_position.x << ", " << m_position.y << ", " << m_position.z << ")\n"<<std::endl;
        std::cout << "Front: (" << m_front.x << ", " << m_front.y << ", " << m_front.z << ")\n"<<std::endl;
        std::cout << "Yaw: " << m_yaw << ", Pitch: " << m_pitch << "\n"<<std::endl;
        std::cout << "FOV: " << m_fov << "\n"<<std::endl;
    }

    //设置各个属性
    void setCameraAttribute(int cameratype, float fov, float yaw, float pitch, float clipNear, float clipFar, glm::vec3& position);    
private:
    // 摄像机属性
    glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 4.0f); // 摄像机位置
    glm::vec3 m_front;    // 摄像机正前方方向
    glm::vec3 m_up;       // 摄像机上方向
    glm::vec3 m_right;    // 摄像机右方向
    glm::vec3 m_worldUp = glm::vec3(0.0f, 1.0f, 0.0f);  // 世界空间中的上方向

    // 视角属性
    float m_yaw = -90.0f;    // 偏航角（水平旋转）yaw = 0：摄像机面向正右方（+X 轴方向）。
                    //yaw = -90：摄像机面向正前方（-Z 轴方向）。
                    // yaw = +90：摄像机面向正后方（+Z 轴方向）。
                    //yaw = ±180：摄像机面向正左方（-X 轴方向）。
    float m_pitch = 0.0f; // 俯仰角（垂直旋转）

    // 投影参数
    float m_fov;          // 视场角（Field of View）
    float m_aspectRatio;  // 宽高比
    float m_nearClip;     // 近裁剪面
    float m_farClip;      // 远裁剪面

    // 移动速度和旋转灵敏度
    float m_movementSpeed;
    float m_mouseSensitivity;

    // 是否使用正交投影
    bool m_isOrthographic;
    float m_orthoSize; // 正交投影的高度范围
};
#endif // CAMERA_H
