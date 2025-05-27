#ifndef UIMATERIAL_H
#define UIMATERIAL_H

#include <QWidget>
#include "myLineEdit.h"
namespace Ui {
class UiMaterial;
}

class UiMaterial : public QWidget
{
    Q_OBJECT

public:
    explicit UiMaterial(QWidget *parent = nullptr);
    ~UiMaterial();
    QPushButton* getBtnBasicColor();
    inline myLineEdit* getLineEditMetallic(){return m_lineEditMetallic;}
    inline myLineEdit* getLineEditRoughness(){return m_lineEditRoughness;}
    inline myLineEdit* getLineEditIOR(){return m_lineEditIOR;}

private slots:
    void on_btn_basicSetting_clicked();

private:
    Ui::UiMaterial *ui;
    QSpacerItem* m_vSpacerItem;
    int m_spacerHeight;
    bool m_isWidgetBasicSettingShow = false;//current status

    myLineEdit* m_lineEditMetallic;
    myLineEdit* m_lineEditRoughness;
    myLineEdit* m_lineEditIOR;
};

#endif // UIMATERIAL_H
