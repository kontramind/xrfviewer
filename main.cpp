#include <QtCore/QUrl>
#include <QtCore/QDir>
#include <QtGui/QGuiApplication>

#include <QtQml/QQmlContext>
#include <QtQml/QQmlApplicationEngine>

#include "xrfcinelooprcv.h"
#include "xrfimageprovider.h"
#include "xrfcineloopmanager.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

//    qmlRegisterType<xrf::CineLoopListModel>("XrfCineLoopModelQml", 1, 0, "XrfCineLoopModel");
//    qmlRegisterType<xrf::CineLoopManager>("XrfCineLoopManagerQml", 1, 0, "XrfCineLoopManager");

    QQmlApplicationEngine engine;
    QQmlContext* context = engine.rootContext();
    auto xrfCineLoopManager = new xrf::CineLoopManager();
    auto xrfImageProvider = new xrf::ImageProvider(&engine, xrfCineLoopManager);
    auto xrfCineLoopRcv = new xrf::CineLoopRcv("C:/dev/data/received/", ".dcm", 104);
    engine.addImageProvider("xrfimage", xrfImageProvider);
    context->setContextProperty("xrfImageProvider", xrfImageProvider);
    context->setContextProperty("xrfCineLoopRcv", xrfCineLoopRcv);
    context->setContextProperty("xrfCineLoopManager", xrfCineLoopManager);
    context->setContextProperty("xrfCineLoopListModel", xrfCineLoopManager->model());

//    QUrl picturesLocationUrl = QUrl::fromLocalFile(QDir::currentPath());
//    context->setContextProperty(QStringLiteral("contextPicturesLocation"), picturesLocationUrl);

    engine.load(QUrl(QLatin1String("qrc:/main2.qml")));

    return app.exec();
}
