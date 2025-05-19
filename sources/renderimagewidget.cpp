#include "renderimagewidget.h"
#include "./ui_renderImageWidget.h"
renderImageWidget::renderImageWidget(cv::Mat& frame, QWidget* parent)
    :QMainWindow(parent), ui(new Ui::renderImageWidget)
{
    ui->setupUi(this); // 初始化 UI
    m_frame = frame.clone();
    resize(1366, 768);

    showImage();
    connect(ui->actionsave, &QAction::triggered, this, &renderImageWidget::slotSaveImage);
}

renderImageWidget::~renderImageWidget() {
    delete ui; // 释放内存
}

void renderImageWidget::showImage()
{
    QImage image = QImage(m_frame.data,                  // 图像数据指针
    m_frame.cols,                  // 宽度
    m_frame.rows,                  // 高度
    static_cast<int>(m_frame.step), // 每行字节数
    QImage::Format_RGB888);         // 格式
    ui->label_Image->setPixmap(QPixmap::fromImage(image));
    ui->label_Image->setAlignment(Qt::AlignCenter);
    ui->label_Image->setGeometry(0, 0, 1281, 720);
}

void renderImageWidget::slotSaveImage()
{
    cv::Mat frame;
    cv::cvtColor(m_frame, frame, cv::COLOR_RGB2BGR);
    cv::imwrite("output.png", frame); // 保存为 PNG 文件
}


