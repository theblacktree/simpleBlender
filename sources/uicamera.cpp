#include "uicamera.h"
#include "sources/ui_uicamera.h"

UiCamera::UiCamera(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UiCamera)
{
    ui->setupUi(this);
    ui->widget_camera->hide();
    m_vSpacerItem = new QSpacerItem(0, m_spacerHeight, QSizePolicy::Minimum, QSizePolicy::Fixed);
    ui->vLayout_main->addSpacerItem(m_vSpacerItem);

    m_lineEditCameraFOV = new myLineEdit(this);
    m_lineEditCameraFOV->setValue(45.00);
    QWidget *placeholderFOV = ui->lineEdit_cameraFOV;
    ui->vLayout_camera->replaceWidget(placeholderFOV, m_lineEditCameraFOV);
    placeholderFOV->deleteLater(); // 删除占位符

    m_lineEditCameraYaw = new myLineEdit(this);
    m_lineEditCameraYaw->setValue(-90.00);
    QWidget *placeholderYaw = ui->lineEdit_cameraYaw;
    ui->vLayout_camera->replaceWidget(placeholderYaw, m_lineEditCameraYaw);
    placeholderYaw->deleteLater(); // 删除占位符

    m_lineEditCameraPitch = new myLineEdit(this);
    m_lineEditCameraPitch->setValue(0.00);
    QWidget *placeholderPitch = ui->lineEdit_cameraPitch;
    ui->vLayout_camera->replaceWidget(placeholderPitch, m_lineEditCameraPitch);
    placeholderPitch->deleteLater(); // 删除占位符

    m_lineEditClipNear = new myLineEdit(this);
    m_lineEditClipNear->setValue(0.10);
    QWidget *placeholderNear = ui->lineEdit_clipNear;
    ui->vLayout_camera->replaceWidget(placeholderNear, m_lineEditClipNear);
    placeholderNear->deleteLater(); // 删除占位符

    m_lineEditClipFar = new myLineEdit(this);
    m_lineEditClipFar->setValue(100.00);
    QWidget *placeholderFar = ui->lineEdit_clipFar;
    ui->vLayout_camera->replaceWidget(placeholderFar, m_lineEditClipFar);
    placeholderFar->deleteLater(); // 删除占位符
}

UiCamera::~UiCamera()
{
    delete ui;
}

QComboBox *UiCamera::getCameraType()
{
    return ui->combo_cameraType;
}

void UiCamera::on_btn_Camera_clicked()
{
    ui->widget_camera->setVisible(!m_isWidgetCameraShow);
    m_isWidgetCameraShow = !m_isWidgetCameraShow;
    if (m_isWidgetCameraShow == true)
    {
        m_spacerHeight -= 300;
        m_vSpacerItem->changeSize(0, m_spacerHeight, QSizePolicy::Minimum, QSizePolicy::Fixed);
        ui->btn_Camera->setText(QString("v基础摄像机设置"));
    }
    else
    {
        m_spacerHeight += 300;
        m_vSpacerItem->changeSize(0, m_spacerHeight, QSizePolicy::Minimum, QSizePolicy::Fixed);
        ui->btn_Camera->setText(QString(">基础摄像机设置"));
    }
    ui->vLayout_main->invalidate();
    ui->vLayout_main->activate();
}

