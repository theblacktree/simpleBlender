#include "mylineedit.h"
void DraggableLineEdit::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        m_dragging = true;
        m_lastPos = event->pos().x();
        // 阻止默认行为，避免进入文本编辑模式
        event->accept();
    }
    else
    {
        QLineEdit::mousePressEvent(event);
    }
}
void DraggableLineEdit::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging) {
        int currentPos = event->pos().x();
        double delta = (currentPos - m_lastPos) / 10.0; // 调整这个除数以控制灵敏度
        double newValue = text().toDouble() + delta;
        setText(QString::number(newValue, 'f', 2)); // 格式化输出
        m_lastPos = currentPos;
        emit valueChanged(newValue); // 发射信号
        // 阻止默认行为
        event->accept();
    }
    else
    {
        QLineEdit::mouseMoveEvent(event);
    }
}
void DraggableLineEdit::mouseReleaseEvent(QMouseEvent *event)
{
    if (m_dragging)
    {
        m_dragging = false;
        // 判断是否为点击（而非拖动）
        QPoint releasePos = event->pos();
        int distance = (m_pressPos - releasePos).manhattanLength();

        if (distance <= m_clickThreshold) {
            // 如果是点击，则进入编辑模式
            setFocus(Qt::MouseFocusReason);
            selectAll(); // 选中所有文本以便于编辑
        }
        // 否则不做任何操作，保持当前状态
        // 阻止默认行为
        event->accept();
    }
    else
    {
        QLineEdit::mouseReleaseEvent(event);
    }

}







/*The following functions being to myLineEdit*/
myLineEdit::myLineEdit(QWidget *parent) : QWidget(parent)
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0); // 移除布局边距
    layout->setSpacing(0); // 移除控件之间的间距

    // 左侧按钮
    m_decreaseButton = new QPushButton("<", this);

    connect(m_decreaseButton, &QPushButton::clicked, this, &myLineEdit::slotDecreaseValue);
    layout->addWidget(m_decreaseButton);

    // 中间的 DraggableLineEdit
    m_lineEdit = new DraggableLineEdit(this);
    m_lineEdit->resize(140,20);
    m_lineEdit->setValidator(new QDoubleValidator(this));
    connect(m_lineEdit, &DraggableLineEdit::valueChanged, this, &myLineEdit::setValue);
    layout->addWidget(m_lineEdit);
    m_lineEdit->setValidator(new QDoubleValidator(this));

    // 右侧按钮
    m_increaseButton = new QPushButton(">", this);

    connect(m_increaseButton, &QPushButton::clicked, this, &myLineEdit::slotIncreaseValue);
    layout->addWidget(m_increaseButton);

    m_lineEdit->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    // 对于按钮，保持默认的固定尺寸策略即可
    m_decreaseButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_increaseButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    m_decreaseButton->setFixedSize(20,20);
    m_increaseButton->setFixedSize(20,20);
    // lineEdit->setTextMargins(0, 0, 0, 0); // 移除文本编辑框的内容边距
    // decreaseButton->setStyleSheet("padding: 0px; margin: 0px;"); // 移除按钮的内边距和外边距
    // increaseButton->setStyleSheet("padding: 0px; margin: 0px;"); // 移除按钮的内边距和外边距
        m_value = 0.0;
}

void myLineEdit::onEditingFinished()
{
    bool ok;
    double newValue = m_lineEdit->text().toDouble(&ok);
    if (ok)
    {
        setValue(newValue);
    }
}
void myLineEdit::setValue(double newValue)
{
    if (newValue == m_value && m_value == 0.00)
    {
        m_lineEdit->setText(QString::number(m_value, 'f', 2)); // 格式化输出
    }
    if (std::abs(newValue - m_value) < 0.00001)
        return ;
    m_value = newValue;
    m_lineEdit->setText(QString::number(m_value, 'f', 2)); // 格式化输出

    emit sigValueChanged(m_value);
}

void myLineEdit::slotIncreaseValue()
{
        setValue(m_value + 0.01);

}

void myLineEdit::slotDecreaseValue()
{
        setValue(m_value - 0.01);
}
