#ifndef UITRANSFORM_H
#define UITRANSFORM_H

#include <QWidget>
#include <QSpacerItem>
#include "myLineEdit.h"
namespace Ui {
class UiTransform;
}

class UiTransform : public QWidget
{
    Q_OBJECT

public:
    explicit UiTransform(QWidget *parent = nullptr);
    ~UiTransform();

    inline myLineEdit* getLineEditTranslationX(){return m_lineEditTranslationX;}
    inline myLineEdit* getLineEditTranslationY(){return m_lineEditTranslationY;}
    inline myLineEdit* getLineEditTranslationZ(){return m_lineEditTranslationZ;}

    inline myLineEdit* getLineEditRotationX(){return m_lineEditRotationX;}
    inline myLineEdit* getLineEditRotationY(){return m_lineEditRotationY;}
    inline myLineEdit* getLineEditRotationZ(){return m_lineEditRotationZ;}

    inline myLineEdit* getLineEditScaleX(){return m_lineEditScaleX;}
    inline myLineEdit* getLineEditScaleY(){return m_lineEditScaleY;}
    inline myLineEdit* getLineEditScaleZ(){return m_lineEditScaleZ;}

private slots:
    void on_btn_translate_clicked();

    void on_btn_rotate_clicked();

    void on_btn_scale_clicked();

private:
    Ui::UiTransform *ui;
    QSpacerItem* m_vSpacerItem;
    int m_spacerHeight = 300;
    bool m_isWidgetTranslateShow = false;//current status
    bool m_isWidgetScaleShow = false;
    bool m_isWidgetRotateShow = false;

    /*myLineEdits*/
    myLineEdit* m_lineEditTranslationX;
    myLineEdit* m_lineEditTranslationY;
    myLineEdit* m_lineEditTranslationZ;

    myLineEdit* m_lineEditRotationX;
    myLineEdit* m_lineEditRotationY;
    myLineEdit* m_lineEditRotationZ;

    myLineEdit* m_lineEditScaleX;
    myLineEdit* m_lineEditScaleY;
    myLineEdit* m_lineEditScaleZ;
};

#endif // UITRANSFORM_H
