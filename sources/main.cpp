#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QSurfaceFormat format;
    format.setVersion(4, 6); // 使用 OpenGL 4.6（或你需要的版本）
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setRenderableType(QSurfaceFormat::OpenGL);
    format.setSamples(4); // 设置为4倍采样，你可以根据需要调整这个值
    QSurfaceFormat::setDefaultFormat(format);
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
