#include "uimaterial.h"
#include "sources/ui_uimaterial.h"

UiMaterial::UiMaterial(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UiMaterial)
{
    ui->setupUi(this);
    m_spacerHeight = 250;
    ui->widget_basicSetting->hide();
    m_vSpacerItem = new QSpacerItem(0, m_spacerHeight, QSizePolicy::Minimum, QSizePolicy::Fixed);
    ui->vLayout_main->addSpacerItem(m_vSpacerItem);

    /*use myLineEdits to replace ui->lineEdit*/
    m_lineEditMetallic = new myLineEdit(this);
    m_lineEditMetallic->setValue(1.00);
    QWidget *placeholderMetallic = ui->lineEdit_metallic;
    ui->vLayout_pbr->replaceWidget(placeholderMetallic, m_lineEditMetallic);
    placeholderMetallic->deleteLater(); // 删除占位符

    m_lineEditRoughness = new myLineEdit(this);
    m_lineEditRoughness->setValue(0.10);
    QWidget *placeholderRoughness = ui->lineEdit_roughness;
    ui->vLayout_pbr->replaceWidget(placeholderRoughness, m_lineEditRoughness);
    placeholderRoughness->deleteLater(); // 删除占位符

    m_lineEditIOR = new myLineEdit(this);
    m_lineEditIOR->setValue(1.450);
    QWidget *placeholderIOR = ui->lineEdit_ior;
    ui->vLayout_pbr->replaceWidget(placeholderIOR, m_lineEditIOR);
    placeholderIOR->deleteLater(); // 删除占位符

}

UiMaterial::~UiMaterial()
{
    delete ui;
}

QPushButton *UiMaterial::getBtnBasicColor()
{
    return ui->btn_basiccolor;
}

void UiMaterial::on_btn_basicSetting_clicked()
{
    ui->widget_basicSetting->setVisible(!m_isWidgetBasicSettingShow);
    m_isWidgetBasicSettingShow = !m_isWidgetBasicSettingShow;
    if (m_isWidgetBasicSettingShow == true)
    {
        m_spacerHeight -= 230;
        m_vSpacerItem->changeSize(0, m_spacerHeight, QSizePolicy::Minimum, QSizePolicy::Fixed);
        ui->btn_basicSetting->setText(QString("v基础材质设置"));
    }
    else
    {
        m_spacerHeight += 230;
        m_vSpacerItem->changeSize(0, m_spacerHeight, QSizePolicy::Minimum, QSizePolicy::Fixed);
        ui->btn_basicSetting->setText(QString(">基础材质设置"));
    }
    ui->vLayout_main->invalidate();
    ui->vLayout_main->activate();
}

