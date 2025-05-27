#ifndef UILIGHT_H
#define UILIGHT_H

#include <QWidget>
#include "myLineEdit.h"
namespace Ui {
class UiLight;
}

class UiLight : public QWidget
{
    Q_OBJECT

public:
    explicit UiLight(QWidget *parent = nullptr);
    ~UiLight();
    QPushButton* getBtnLightColor();
    inline myLineEdit* getLineEditLightIntensity(){return m_lineEditLightIntensity;}
    inline myLineEdit* getLineEditLightLocateX(){return m_lineEditLightLocateX;}
    inline myLineEdit* getLineEditLightLocateY(){return m_lineEditLightLocateY;}
    inline myLineEdit* getLineEditLightLocateZ(){return m_lineEditLightLocateZ;}
    inline myLineEdit* getLineEditLightQuadratic(){return m_lineEditLightQuadratic;}
    inline myLineEdit* getLineEditLightLiner(){return m_lineEditLightLiner;}
    inline myLineEdit* getLineEditLightConstant(){return m_lineEditLightConstant;}
    inline myLineEdit* getLineEditLightDirectionX(){return m_lineEditLightDirectionX;}
    inline myLineEdit* getLineEditLightDirectionY(){return m_lineEditLightDirectionY;}
    inline myLineEdit* getLineEditLightDirectionZ(){return m_lineEditLightDirectionZ;}
    inline myLineEdit* getLineEditLightInnerConeAngle(){return m_lineEditLightInnerConeAngle;}
    inline myLineEdit* getLineEditLightOuterConeAngle(){return m_lineEditLightOuterConeAngle;}

private slots:
    void on_btn_location_clicked();

    void on_btn_attenuation_clicked();

    void on_btn_direction_clicked();

    void on_btn_basicSetting_clicked();

    void on_btn_Angle_clicked();

private:
    Ui::UiLight *ui;
    QSpacerItem* m_vSpacerItem;
    int m_spacerHeight;
    bool m_isWidgetbasicSeetingShow = false;//current status
    bool m_isWidgetLocationShow = false;
    bool m_isWidgetAttenuationShow = false;
    bool m_isWidgetDirectionShow = false;
    bool m_isWidgetSpotShow = false;
    /*light intemsity*/
    myLineEdit* m_lineEditLightIntensity;
    /*light location*/
    myLineEdit* m_lineEditLightLocateX;
    myLineEdit* m_lineEditLightLocateY;
    myLineEdit* m_lineEditLightLocateZ;
    /*light atteution*/
    myLineEdit* m_lineEditLightQuadratic;
    myLineEdit* m_lineEditLightLiner;
    myLineEdit* m_lineEditLightConstant;

    myLineEdit* m_lineEditLightDirectionX;
    myLineEdit* m_lineEditLightDirectionY;
    myLineEdit* m_lineEditLightDirectionZ;

    myLineEdit* m_lineEditLightInnerConeAngle;
    myLineEdit* m_lineEditLightOuterConeAngle;
};

#endif // UILIGHT_H
