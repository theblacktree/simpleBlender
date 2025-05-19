#ifndef GLWIDGET_H
#define GLWIDGET_H
//这个类是管理物体和灯光的类
#include "texturemanager.h"
#define glCheckError() glCheckError_(__FILE__, __LINE__)

#include <QOpenGLWidget>
#include <QOpenGLExtraFunctions>
#include <unordered_map>
#include <QObject>
#include <QWidget>
#include <QFile>
#include <map>
#include <algorithm>
#include <string>
#include "object.h"
#include "light.h"
#include "Camera.h"
#include <opencv2/opencv.hpp>
#include "texturemanager.h"
#define MAX_LIGHTS 50

class MainWindow;
class GLWidget: public QOpenGLWidget, protected QOpenGLExtraFunctions
{
    Q_OBJECT
public:
    GLWidget(MainWindow *mainWindow, QWidget *parent = nullptr);
    ~GLWidget();
    // 添加物体到场景中
    void addObject(Object *object, QString objectName);
    // 获取物体（通过索引）
    Object* getObject(const std::string& str);
    // 获取物体数量
    int getObjectCount() const;
//后续添加灯光的操作存储删除等
    void addLight(Light* light, int lightId);

    Light* getLight(const std::string& str);

    void addCamera(Camera* camera, int cameraId);

    Camera* getCamera(const std::string& str);

    template<typename T>
    T queryLastItem(std::string& strLastItemName);
    //捕获帧缓冲，图像信息
    cv::Mat captureFrame();

    //添加着色器,2 arguments are in, last two arguments are out
    void addShader(QString& vertexPath, QString& fragmentPath, GLuint& vertexShader, GLuint& fragmentShader);

    //更新物体/灯光/摄像机的名称
    void renameOneItem(const std::string oldStr, const std::string newStr);
protected:
    void initializeGL() override;
    void paintGL() override;
    void resizeGL(int w, int h) override;

private slots:
    //删除所有选中的物体，包括物体，灯光，摄像机等
    void slotRemoveObjects(const std::vector<std::string> &strRemove);

    //改变选中目标的model矩阵    // 设置物体的模型矩阵
    void slotSetModelMatrix(const std::vector<std::string> &strvec, glm::vec3& translationXYZ, glm::vec3& rotationXYZ, glm::vec3& scaleXYZ);

    void slotSetColor(const std::vector<std::string>&selectedVec, QColor& color);

    void slotSetMaterialValue(const std::vector<std::string>&selectedVec, float metallic, float roughness, float ior);

    void slotSwitchCamera(const std::vector<std::string> &selectedVec);

    void slotSetCameraAttribute(const std::vector<std::string>&strSelectedCamera, int cameratype, float fov, float yaw, float pitch, float clipNear, float clipFar, glm::vec3& position);

    GLenum glCheckError_(const char *file, int line);

    void slotSetLightAttribute(const std::vector<std::string>&strSelectedLight, ShaderLight& sl);

    void slotSetObjecxtUVTexture(const std::string& selectedString, std::wstring& textureFilePath, TextureType textureType);

    void slotRemoveObjectUVTexture(const std::string& selectedString, TextureType textureType);

    void slotCreateHDRMAP(std::wstring& HDRFilePath);
private:
    std::unordered_map<std::string, Object*> m_objects; // 物体类物体列表
    std::map<std::string, Light*> m_lights; // 灯光类物体列表
    std::map<std::string, Camera*> m_cameras; // 摄像机类物体列表
    MainWindow *m_mainWindow;

private:
    QString m_objectVertexPath = ":/cube/vertex.vert";
    QString m_objectFragmentPath = ":/cube/frag.frag";
    GLuint m_objectProgram;

    QString m_skyboxVertexPath = ":/skybox/skybox.vert";
    QString m_skyboxFragmentPath = ":/skybox/skybox.frag";
    GLuint m_skyboxProgram;

    std::vector<ShaderLight> allLights;
    //维护一个当前视角的摄像机
    Camera* m_defaultCamera;
    Camera* m_currentCamera;

    GLuint m_ubo;

};
template<typename T>
T GLWidget::queryLastItem(std::string &strLastItemName)
{
    T ptr = nullptr;
    if constexpr (std::is_same_v<T, Object*>)
    {
        if (m_objects.count(strLastItemName))
            ptr = m_objects[strLastItemName];
    }
    if constexpr (std::is_same_v<T, Camera*>)
    {
        if (m_cameras.count(strLastItemName))
            ptr = m_cameras[strLastItemName];
        return ptr;
    }
    if constexpr (std::is_same_v<T, Light*>)
    {
        for (auto& pair : m_lights)
        {
            if (pair.first == strLastItemName)
            {
                ptr = pair.second;
                return ptr;
            }
        }
    }
    return ptr;
}
#endif // GLWIDGET_H
