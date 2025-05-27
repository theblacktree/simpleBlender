#include "uitransform.h"
//#include "./uitransform.h"
#include "sources/ui_uitransform.h"

UiTransform::UiTransform(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::UiTransform)
{
    ui->setupUi(this);
    ui->widget_translate->hide();
    ui->widget_rotate->hide();
    ui->widget_scale->hide();
    m_vSpacerItem = new QSpacerItem(0, m_spacerHeight, QSizePolicy::Minimum, QSizePolicy::Fixed);
    ui->vLayout_main->addSpacerItem(m_vSpacerItem);

    /*use myLineEdits to replace ui->lineEdit*/
    m_lineEditTranslationX = new myLineEdit(this);
    m_lineEditTranslationX->setValue(0.00);
    QWidget *placeholderTranslationX = ui->lineEdit_translationX;
    ui->vLayoutTranslate->replaceWidget(placeholderTranslationX, m_lineEditTranslationX);
    placeholderTranslationX->deleteLater(); // 删除占位符

    m_lineEditTranslationY = new myLineEdit(this);
    m_lineEditTranslationY->setValue(0.00);
    QWidget *placeholderTranslationY = ui->lineEdit_translationY;
    ui->vLayoutTranslate->replaceWidget(placeholderTranslationY, m_lineEditTranslationY);
    placeholderTranslationY->deleteLater(); // 删除占位符

    m_lineEditTranslationZ = new myLineEdit(this);
    m_lineEditTranslationZ->setValue(0.00);
    QWidget *placeholderTranslationZ = ui->lineEdit_translationZ;
    ui->vLayoutTranslate->replaceWidget(placeholderTranslationZ, m_lineEditTranslationZ);
    placeholderTranslationZ->deleteLater(); // 删除占位符

    m_lineEditRotationX = new myLineEdit(this);
    m_lineEditRotationX->setValue(0.00);
    QWidget *placeholderRotationX = ui->lineEdit_rotationX;
    ui->vLayoutRotate->replaceWidget(placeholderRotationX, m_lineEditRotationX);
    placeholderRotationX->deleteLater(); // 删除占位符

    m_lineEditRotationY = new myLineEdit(this);
    m_lineEditRotationY->setValue(0.00);
    QWidget *placeholderRotationY = ui->lineEdit_rotationY;
    ui->vLayoutRotate->replaceWidget(placeholderRotationY, m_lineEditRotationY);
    placeholderRotationY->deleteLater(); // 删除占位符

    m_lineEditRotationZ = new myLineEdit(this);
    m_lineEditRotationZ->setValue(0.00);
    QWidget *placeholderRotationZ = ui->lineEdit_rotationZ;
    ui->vLayoutRotate->replaceWidget(placeholderRotationZ, m_lineEditRotationZ);
    placeholderRotationZ->deleteLater(); // 删除占位符

    m_lineEditScaleX = new myLineEdit(this);
    m_lineEditScaleX->setValue(1.00);
    QWidget *placeholderScaleX = ui->lineEdit_scaleX;
    ui->vLayoutScale->replaceWidget(placeholderScaleX, m_lineEditScaleX);
    placeholderScaleX->deleteLater(); // 删除占位符

    m_lineEditScaleY = new myLineEdit(this);
    m_lineEditScaleY->setValue(1.00);
    QWidget *placeholderScaleY = ui->lineEdit_scaleY;
    ui->vLayoutScale->replaceWidget(placeholderScaleY, m_lineEditScaleY);
    placeholderScaleY->deleteLater(); // 删除占位符

    m_lineEditScaleZ = new myLineEdit(this);
    m_lineEditScaleZ->setValue(1.00);
    QWidget *placeholderScaleZ = ui->lineEdit_scaleZ;
    ui->vLayoutScale->replaceWidget(placeholderScaleZ, m_lineEditScaleZ);
    placeholderScaleZ->deleteLater(); // 删除占位符

}

UiTransform::~UiTransform()
{
    delete ui;
}

void UiTransform::on_btn_translate_clicked()
{
    ui->widget_translate->setVisible(!m_isWidgetTranslateShow);
    m_isWidgetTranslateShow = !m_isWidgetTranslateShow;
    if (m_isWidgetTranslateShow == true)
    {
        m_spacerHeight -= 85;
        m_vSpacerItem->changeSize(0, m_spacerHeight, QSizePolicy::Minimum, QSizePolicy::Fixed);
        ui->btn_translate->setText(QString("v平移"));
    }
    else
    {
        m_spacerHeight += 85;
        m_vSpacerItem->changeSize(0, m_spacerHeight, QSizePolicy::Minimum, QSizePolicy::Fixed);
        ui->btn_translate->setText(QString(">平移"));
    }
    ui->vLayout_main->invalidate();
    ui->vLayout_main->activate();
}


void UiTransform::on_btn_rotate_clicked()
{
    ui->widget_rotate->setVisible(!m_isWidgetRotateShow);
    m_isWidgetRotateShow = !m_isWidgetRotateShow;
    if (m_isWidgetRotateShow == true)
    {
        m_spacerHeight -= 85;
        m_vSpacerItem->changeSize(0, m_spacerHeight, QSizePolicy::Minimum, QSizePolicy::Fixed);
        ui->btn_rotate->setText(QString("v旋转"));
    }
    else
    {
        m_spacerHeight += 85;
        m_vSpacerItem->changeSize(0, m_spacerHeight, QSizePolicy::Minimum, QSizePolicy::Fixed);
        ui->btn_rotate->setText(QString(">旋转"));
    }
    ui->vLayout_main->invalidate();
    ui->vLayout_main->activate();
}


void UiTransform::on_btn_scale_clicked()
{
    ui->widget_scale->setVisible(!m_isWidgetScaleShow);
    m_isWidgetScaleShow = !m_isWidgetScaleShow;
    if (m_isWidgetScaleShow == true)
    {
        m_spacerHeight -= 85;
        m_vSpacerItem->changeSize(0, m_spacerHeight, QSizePolicy::Minimum, QSizePolicy::Fixed);
        ui->btn_scale->setText(QString("v缩放"));
    }
    else
    {
        m_spacerHeight += 85;
        m_vSpacerItem->changeSize(0, m_spacerHeight, QSizePolicy::Minimum, QSizePolicy::Fixed);
        ui->btn_scale->setText(QString(">缩放"));
    }
    ui->vLayout_main->invalidate();
    ui->vLayout_main->activate();
}

