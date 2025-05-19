#include "camera.h"
using namespace  std;

Camera::Camera(glm::vec3 position ,glm::vec3 worldUp ,float yaw , float pitch)
    : m_position(position), m_worldUp(worldUp), m_yaw(yaw), m_pitch(pitch),
    m_front(glm::vec3(0.0f, 0.0f, -1.0f)), m_movementSpeed(2.5f),
    m_mouseSensitivity(0.1f), m_fov(45.0f), m_aspectRatio(16.0f / 9.0f),
    m_nearClip(0.1f), m_farClip(100.0f), m_isOrthographic(false), m_orthoSize(10.0f)
{
    updateCameraVectors();
}

void Camera::updateCameraVectors()
{
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    newFront.y = sin(glm::radians(m_pitch));
    newFront.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
    m_front = glm::normalize(newFront);

    // 计算右向量和上向量
    m_right = glm::normalize(glm::cross(m_front, m_worldUp));
    m_up = glm::normalize(glm::cross(m_right, m_front));
}

glm::mat4 Camera::getProjectionMatrix() const
{
    if (m_isOrthographic) {//正交投影
        float halfSize = m_orthoSize * 0.5f;
        return glm::ortho(-halfSize * m_aspectRatio, halfSize * m_aspectRatio,
                          -halfSize, halfSize, m_nearClip, m_farClip);
    } else {//透射投影
        return glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearClip, m_farClip);
    }
}

void Camera::processKeyboard(CameraMovement direction, float deltaTime)
{
    float velocity = m_movementSpeed * deltaTime;
    if (direction == FORWARD)
        m_position += m_front * velocity;
    if (direction == BACKWARD)
        m_position -= m_front * velocity;
    if (direction == LEFT)
        m_position -= m_right * velocity;
    if (direction == RIGHT)
        m_position += m_right * velocity;
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= m_mouseSensitivity;
    yoffset *= m_mouseSensitivity;

    m_yaw += xoffset;
    m_pitch += yoffset;

    // 防止摄像机翻转
    if (constrainPitch) {
        if (m_pitch > 89.0f)
            m_pitch = 89.0f;
        if (m_pitch < -89.0f)
            m_pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::setCameraAttribute(int cameratype, float fov, float yaw, float pitch, float clipNear, float clipFar, glm::vec3 &position)
{
    if (cameratype == 0)
        m_isOrthographic = false;
    else if (cameratype == 1)
        m_isOrthographic = true;
    else
    {
        //全景模式
    }
    m_fov = fov;
    m_yaw = yaw;
    m_pitch = pitch;
    m_nearClip = clipNear;
    m_farClip = clipFar;
    m_position = position;
    updateCameraVectors();
}
