#include "uilight.h"
#include "sources/ui_uilight.h"

UiLight::UiLight(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UiLight)
{
    ui->setupUi(this);
    ui->widget_basicSeeting->hide();
    ui->widget_location->hide();
    ui->widget_attenuation->hide();
    ui->widget_direction->hide();
    ui->widget_spot->hide();
    m_spacerHeight = 430;
    m_vSpacerItem = new QSpacerItem(0, m_spacerHeight, QSizePolicy::Minimum, QSizePolicy::Fixed);
    ui->vLayout_main->addSpacerItem(m_vSpacerItem);

    m_lineEditLightIntensity = new myLineEdit(this);
    m_lineEditLightIntensity->setValue(10.00);
    QWidget *placeholderIntensity = ui->lineEdit_lightIntensity;
    ui->vLayout_basicSetting->replaceWidget(placeholderIntensity, m_lineEditLightIntensity);
    placeholderIntensity->deleteLater(); // 删除占位符

    m_lineEditLightLocateX = new myLineEdit(this);
    m_lineEditLightLocateX->setValue(0.00);
    QWidget *placeholderLocateX = ui->lineEdit_lightLocateX;
    ui->vLayout_location->replaceWidget(placeholderLocateX, m_lineEditLightLocateX);
    placeholderLocateX->deleteLater(); // 删除占位符

    m_lineEditLightLocateY = new myLineEdit(this);
    m_lineEditLightLocateY->setValue(0.00);
    QWidget *placeholderLocateY = ui->lineEdit_lightLocateY;
    ui->vLayout_location->replaceWidget(placeholderLocateY, m_lineEditLightLocateY);
    placeholderLocateY->deleteLater(); // 删除占位符

    m_lineEditLightLocateZ = new myLineEdit(this);
    m_lineEditLightLocateZ->setValue(3.00);
    QWidget *placeholderLocateZ = ui->lineEdit_lightLocateZ;
    ui->vLayout_location->replaceWidget(placeholderLocateZ, m_lineEditLightLocateZ);
    placeholderLocateZ->deleteLater(); // 删除占位符

    m_lineEditLightQuadratic = new myLineEdit(this);
    m_lineEditLightQuadratic->setValue(0.032);
    QWidget *placeholderQuadratic = ui->lineEdit_quadratic;
    ui->vLayout_attenuation->replaceWidget(placeholderQuadratic, m_lineEditLightQuadratic);
    placeholderQuadratic->deleteLater(); // 删除占位符

    m_lineEditLightLiner = new myLineEdit(this);
    m_lineEditLightLiner->setValue(0.09);
    QWidget *placeholderLiner = ui->lineEdit_liner;
    ui->vLayout_attenuation->replaceWidget(placeholderLiner, m_lineEditLightLiner);
    placeholderLiner->deleteLater(); // 删除占位符

    m_lineEditLightConstant = new myLineEdit(this);
    m_lineEditLightConstant->setValue(1.00);
    QWidget *placeholderConstant = ui->lineEdit_constant;
    ui->vLayout_attenuation->replaceWidget(placeholderConstant, m_lineEditLightConstant);
    placeholderConstant->deleteLater(); // 删除占位符

    m_lineEditLightDirectionX = new myLineEdit(this);
    m_lineEditLightDirectionX->setValue(0.00);
    QWidget *placeholderDirectionX = ui->lineEdit_lightDirectionX;
    ui->vLayout_direction->replaceWidget(placeholderDirectionX, m_lineEditLightDirectionX);
    placeholderDirectionX->deleteLater(); // 删除占位符

    m_lineEditLightDirectionY = new myLineEdit(this);
    m_lineEditLightDirectionY->setValue(0.00);
    QWidget *placeholderDirectionY = ui->lineEdit_lightDirectionY;
    ui->vLayout_direction->replaceWidget(placeholderDirectionY, m_lineEditLightDirectionY);
    placeholderDirectionY->deleteLater(); // 删除占位符

    m_lineEditLightDirectionZ = new myLineEdit(this);
    m_lineEditLightDirectionZ->setValue(1.00);
    QWidget *placeholderDirectionZ = ui->lineEdit_lightDirectionZ;
    ui->vLayout_direction->replaceWidget(placeholderDirectionZ, m_lineEditLightDirectionZ);
    placeholderDirectionZ->deleteLater(); // 删除占位符

    m_lineEditLightInnerConeAngle = new myLineEdit(this);
    m_lineEditLightInnerConeAngle->setValue(45.00);
    QWidget *placeholderInnerConeAngle = ui->lineEdit_lightInnerConeAngle;
    ui->vLayout_angle->replaceWidget(placeholderInnerConeAngle, m_lineEditLightInnerConeAngle);
    placeholderInnerConeAngle->deleteLater(); // 删除占位符

    m_lineEditLightOuterConeAngle = new myLineEdit(this);
    m_lineEditLightOuterConeAngle->setValue(60.00);
    QWidget *placeholderOuterConeAngle = ui->lineEdit_lightOuterConeAngle;
    ui->vLayout_angle->replaceWidget(placeholderOuterConeAngle, m_lineEditLightOuterConeAngle);
    placeholderOuterConeAngle->deleteLater(); // 删除占位符
}

UiLight::~UiLight()
{
    delete ui;
}

QPushButton *UiLight::getBtnLightColor()
{
    return ui->btn_lightColor;
}

void UiLight::on_btn_location_clicked()
{
    ui->widget_location->setVisible(!m_isWidgetLocationShow);
    m_isWidgetLocationShow = !m_isWidgetLocationShow;
    if (m_isWidgetLocationShow == true)
    {
        m_spacerHeight -= 95;
        m_vSpacerItem->changeSize(0, m_spacerHeight, QSizePolicy::Minimum, QSizePolicy::Fixed);
        ui->btn_location->setText(QString("v位置（点光源/面光源/聚光灯）"));
    }
    else
    {
        m_spacerHeight += 95;
        m_vSpacerItem->changeSize(0, m_spacerHeight, QSizePolicy::Minimum, QSizePolicy::Fixed);
        ui->btn_location->setText(QString(">位置（点光源/面光源/聚光灯）"));
    }
    ui->vLayout_main->invalidate();
    ui->vLayout_main->activate();
}


void UiLight::on_btn_attenuation_clicked()
{
    ui->widget_attenuation->setVisible(!m_isWidgetAttenuationShow);
    m_isWidgetAttenuationShow = !m_isWidgetAttenuationShow;
    if (m_isWidgetAttenuationShow == true)
    {
        m_spacerHeight -= 95;
        m_vSpacerItem->changeSize(0, m_spacerHeight, QSizePolicy::Minimum, QSizePolicy::Fixed);
        ui->btn_attenuation->setText(QString("v衰减因子（点光源/面光源/聚光灯）"));
    }
    else
    {
        m_spacerHeight += 95;
        m_vSpacerItem->changeSize(0, m_spacerHeight, QSizePolicy::Minimum, QSizePolicy::Fixed);
        ui->btn_attenuation->setText(QString(">衰减因子（点光源/面光源/聚光灯）"));
    }
    ui->vLayout_main->invalidate();
    ui->vLayout_main->activate();
}

void UiLight::on_btn_direction_clicked()
{
    ui->widget_direction->setVisible(!m_isWidgetDirectionShow);
    m_isWidgetDirectionShow = !m_isWidgetDirectionShow;
    if (m_isWidgetDirectionShow == true)
    {
        m_spacerHeight -= 95;
        m_vSpacerItem->changeSize(0, m_spacerHeight, QSizePolicy::Minimum, QSizePolicy::Fixed);
        ui->btn_direction->setText(QString("v光源方向（方向光源/面光源/聚光灯）"));
    }
    else
    {
        m_spacerHeight += 95;
        m_vSpacerItem->changeSize(0, m_spacerHeight, QSizePolicy::Minimum, QSizePolicy::Fixed);
        ui->btn_direction->setText(QString(">光源方向（方向光源/面光源/聚光灯）"));
    }
    ui->vLayout_main->invalidate();
    ui->vLayout_main->activate();
}

void UiLight::on_btn_basicSetting_clicked()
{
    ui->widget_basicSeeting->setVisible(!m_isWidgetbasicSeetingShow);
    m_isWidgetbasicSeetingShow = !m_isWidgetbasicSeetingShow;
    if (m_isWidgetbasicSeetingShow == true)
    {
        m_spacerHeight -= 75;
        m_vSpacerItem->changeSize(0, m_spacerHeight, QSizePolicy::Minimum, QSizePolicy::Fixed);
        ui->btn_basicSetting->setText(QString("v基础光源设置"));
    }
    else
    {
        m_spacerHeight += 75;
        m_vSpacerItem->changeSize(0, m_spacerHeight, QSizePolicy::Minimum, QSizePolicy::Fixed);
        ui->btn_basicSetting->setText(QString(">基础光源设置"));
    }
    ui->vLayout_main->invalidate();
    ui->vLayout_main->activate();
}


void UiLight::on_btn_Angle_clicked()
{
    ui->widget_spot->setVisible(!m_isWidgetSpotShow);
    m_isWidgetSpotShow = !m_isWidgetSpotShow;
    if (m_isWidgetSpotShow == true)
    {
        m_spacerHeight -= 95;
        m_vSpacerItem->changeSize(0, m_spacerHeight, QSizePolicy::Minimum, QSizePolicy::Fixed);
        ui->btn_Angle->setText(QString("v内外锥角（聚光灯）"));
    }
    else
    {
        m_spacerHeight += 95;
        m_vSpacerItem->changeSize(0, m_spacerHeight, QSizePolicy::Minimum, QSizePolicy::Fixed);
        ui->btn_Angle->setText(QString(">内外锥角（聚光灯）"));
    }
    ui->vLayout_main->invalidate();
    ui->vLayout_main->activate();
}

