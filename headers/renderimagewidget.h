#ifndef RENDERIMAGEWIDGET_H
#define RENDERIMAGEWIDGET_H
#include <QMainWindow>
#include <QObject>
#include <opencv2/opencv.hpp>
QT_BEGIN_NAMESPACE
namespace Ui {
class renderImageWidget;
}
QT_END_NAMESPACE
class renderImageWidget: public QMainWindow
{
    Q_OBJECT
public:
    renderImageWidget(cv::Mat& frame, QWidget* parent = nullptr );
    ~renderImageWidget();
    void showImage();

public slots:
    void slotSaveImage();

private:
    Ui::renderImageWidget *ui;
    cv::Mat m_frame;
};

#endif // RENDERIMAGEWIDGET_H
