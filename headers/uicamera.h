#ifndef UICAMERA_H
#define UICAMERA_H

#include <QWidget>
#include <QSpacerItem>
#include <QComboBox>
#include "myLineEdit.h"

namespace Ui {
class UiCamera;
}

class UiCamera : public QWidget
{
    Q_OBJECT

public:
    explicit UiCamera(QWidget *parent = nullptr);
    ~UiCamera();

    //getAll the input QObjects
    QComboBox* getCameraType();
    inline myLineEdit* getLineEditCameraFOV(){return m_lineEditCameraFOV;}
    inline myLineEdit* getLineEditCameraYaw(){return m_lineEditCameraYaw;}
    inline myLineEdit* getLineEditCameraPitch(){return m_lineEditCameraPitch;}
    inline myLineEdit* getLineEditClipNear(){return m_lineEditClipNear;}
    inline myLineEdit* getLineEditClipFar(){return m_lineEditClipFar;}

    inline myLineEdit* getLineEditLocationX(){return m_lineEditLocationX;}
    inline myLineEdit* getLineEditLocationY(){return m_lineEditLocationY;}
    inline myLineEdit* getLineEditLocationZ(){return m_lineEditLocationZ;}

private slots:
    void on_btn_basicSetting_clicked();

    void on_btn_cameraPosition_clicked();

private:
    Ui::UiCamera *ui;
    QSpacerItem* m_vSpacerItem;
    int m_spacerHeight = 450;
    bool m_isWidgetBasicSettingShow = false;//current status
    bool m_isWidgetLocationShow = false;

    /*The following varietys are myLineEdits.*/
    myLineEdit* m_lineEditCameraFOV;
    myLineEdit* m_lineEditCameraYaw;
    myLineEdit* m_lineEditCameraPitch;
    myLineEdit* m_lineEditClipNear;
    myLineEdit* m_lineEditClipFar;

    myLineEdit* m_lineEditLocationX;
    myLineEdit* m_lineEditLocationY;
    myLineEdit* m_lineEditLocationZ;
};

#endif // UICAMERA_H
