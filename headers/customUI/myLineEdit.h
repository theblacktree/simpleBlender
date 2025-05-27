#ifndef MYLINEEDIT_H
#define MYLINEEDIT_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QDoubleValidator>

#include <QLineEdit>
#include <QMouseEvent>
#include <QDoubleValidator>

#include <QLineEdit>
#include <QMouseEvent>
#include <QDoubleValidator>
#include <math.h>

class DraggableLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit DraggableLineEdit(QWidget *parent = nullptr) : QLineEdit(parent), m_dragging(false), m_lastPos(0)
    {

    }
protected:
    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

signals:
    void valueChanged(double value);
private:
    bool m_dragging;
    int m_lastPos;
    QPoint m_pressPos; // 记录鼠标按下时的位置
    const int m_clickThreshold = 5; // 定义一个阈值来区分点击和拖动,px
};

class myLineEdit : public QWidget
{
    Q_OBJECT
public:
    explicit myLineEdit(QWidget *parent = nullptr);
private slots:
    void slotIncreaseValue();

    void slotDecreaseValue();// 可以根据需要调整减量

    void onEditingFinished();

signals:
    void sigValueChanged(double value);

public:
    void setValue(double newValue);
    inline double getValue(){return m_value;}

private:
    DraggableLineEdit* m_lineEdit;
    QPushButton* m_decreaseButton;
    QPushButton *m_increaseButton;
    double m_value;
};
#endif // MYLINEEDIT_H

