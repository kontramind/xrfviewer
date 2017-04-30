#include <QtCore/QUrl>
#include <QtCore/QDir>
#include <QtGui/QGuiApplication>

#include <QtQml/QQmlContext>
#include <QtQml/QQmlApplicationEngine>

#include "xrfimageprovider.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    QQmlContext* context = engine.rootContext();

    engine.addImageProvider("xrfimage", new xrf::XRFImageProvider(&engine));

    QUrl picturesLocationUrl = QUrl::fromLocalFile(QDir::currentPath());
    context->setContextProperty(QStringLiteral("contextPicturesLocation"), picturesLocationUrl);

    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    return app.exec();
}
