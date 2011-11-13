#include <QtGui/QApplication>
#include "qmlapplicationviewer.h"
#include "mer.h"

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));
    Mer mer;
    mer.showFullScreen();
    /*
    QScopedPointer<QmlApplicationViewer> viewer(QmlApplicationViewer::create());

    viewer->setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer->setMainQmlFile(QLatin1String(":qml/merux/main.qml"));
    viewer->showExpanded();
    */
    return app->exec();
}
