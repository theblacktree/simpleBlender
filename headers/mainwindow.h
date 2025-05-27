#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTreeView>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QKeyEvent>
#include <QMessageBox>
#include <QItemSelectionModel>
#include <unordered_set>
#include <QColorDialog>
#include <QFileDialog>
#include "cube.h"
#include "uvsphere.h"
#include "torus.h"
#include "pointlight.h"
#include "directionallight.h"
#include "spotlight.h"
#include "glwidget.h"
#include "renderimagewidget.h"
#include "mystandarditemmodel.h"
#include "uitransform.h"
#include "uicamera.h"
#include "uimaterial.h"
#include "uilight.h"
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    /*****************
     * Function Name: resizeEvent
     * Function Description: resize Widget size and set Widget location
     * Function Parameters: resize event
     * Function return : void
     */
    void resizeEvent(QResizeEvent *event)override;

    /*****************
     * Function Name: keyReleaseEvent
     * Function Description: This function works when you press and release key
     *         like Delete_key and Enter_key,It can be called when you change object's/camera's/lights's attribute or delete objects/
     *         cameras/lights instance.
     * Function Parameters: keyboard event
     * Function return : void
     */
    void keyReleaseEvent(QKeyEvent* ev)override;

//获得实例管理选中条目的名字合集，用于各函数中调用并发射各类信号
    std::vector<std::string> getSelectedVector();

    void resetObjectId();
    void resetLightId();
    void resetCameraId();

    void showObjectInfo(Object* ptr);
    void showLightInfo(Light* ptr);
    void showCameraInfo(Camera* ptr);

private:
    /*****************
     * Function Name: removeItems
     * Function Description: This function works when you press and release key Delete
     *         It is called by function keyReleaseEvent, when you delete objects/cameras/lights,
     *         it will collect all the Items from treeView and send signal to class GLWidget.
     * Function Parameters: void
     * Function return : void
     */
    void removeItems();

    /*****************
     * Function Name: setdisableAttributeWidget
     * Function Description: This function called when you want to diable all the children objects
     * on the widget.
     * Function Parameters: widget:you want to control; disabled:true or false
     * Function return : void
     */
    void setdisableAttributeWidget(QWidget *widget, bool disabled);

private slots:

    void slotChangeItemName(const QModelIndex &index, const QString &oldValue, const QString &newValue);
    //创建物体
    void slotCreateObject(bool checked);
    //创建灯光
    void slotCreateLight();
    //改变物体颜色
    void slotPickColor();
    //创建摄像机
    void slotCreateCamera();
    //切换摄像机
    void slotSwitchCamera();

    void slotSelectionChanged(const QItemSelection& selected, const QItemSelection& deselected);
//由QAction进行渲染的图像，新开窗体
    void slotRenderImage();

    void slotOpenUVTextureDialog();

    void slotpassUVTextureInfo(const QString& s);

    void slotRemoveUVTexture();

    void slotCameraAttributeChanged();

    void slotChangeObjectModel();

    void slotChangeObjectMaterial();
    //change lightSetting
    void slotChangeLightSetting();


signals:
    void sigRemove(const std::vector<std::string>&strRemove);

    void sigSetModel(const std::vector<std::string>& selectedVec, glm::vec3& translationXYZ, glm::vec3& rotationXYZ, glm::vec3& scaleXYZ);

    void sigColorSelected(const std::vector<std::string>&selectedVec, QColor& selectedColor);

    void sigSetMaterialValue(const std::vector<std::string>&selectedVec, float metallic, float roughness, float ior);

    void sigSwitchCamera(const std::vector<std::string>&strSelectedCamera);

    void sigSetCameraAttribute(const std::vector<std::string>&strSelectedCamera, int cameratype, float fov, float yaw, float pitch, float clipNear, float clipFar, glm::vec3& position);

    void sigSetLightAttribute(const std::vector<std::string>&strSelectedLight, ShaderLight& sl);

    void sigSetObjectUVTexture(const std::string& selectedString, std::wstring& textureFilePath, TextureType textureType);

    void sigRemoveUVTextures(const std::string& selectedString, TextureType textureType);

    void sigCreateHDRMap(std::wstring& HDRtextureFilePath);
private:
    Ui::MainWindow *ui;

    GLWidget* m_glwidget;

    renderImageWidget* m_renderImageWidget;

    QColorDialog* m_colorDialog;
    std::unique_ptr<QFileDialog> m_fileDialog;

    //QStandardItemModel* m_model;
    MyStandardItemModel* m_model;

    QStandardItem* m_itemObject;
    QStandardItem* m_itemLight;
    QStandardItem* m_itemCamera;
    //立方体物体的唯一键
    int m_objectId = 1;
    //灯光唯一键
    int m_lightId= 1;
    //摄像机唯一键
    int m_cameraId= 1;

    //widget
    UiTransform* m_uitransform;
    UiCamera* m_uicamera;
    UiMaterial* m_uimaterial;
    UiLight* m_uilight;
};
#endif // MAINWINDOW_H
