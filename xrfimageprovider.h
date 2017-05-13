#ifndef XRFIMAGEPROVIDER_H
#define XRFIMAGEPROVIDER_H

#include <QFont>
#include <QImage>
#include <QDebug>
#include <QPixmap>
#include <QString>
#include <QPainter>
#include <QQmlEngine>
#include <QThreadPool>
#include <QQuickImageProvider>

#include <memory>

#include"xrfcineloopmanager.h"

class QQmlApplicationEngine;

namespace xrf {

    class CineLoop;

    class ImageProvider : public QObject, public QQuickImageProvider
    {
        Q_OBJECT
    public:
        ImageProvider(QQmlApplicationEngine* qmlAppEngine, CineLoopManager* manager);
        QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);
        const QString getFrameDcmKeyTagValuesAsHtml() const;

    public slots:
//        void setCineLoop(CineLoop* loop);

    private:
        CineLoopManager* mCineLoopManager{ nullptr };
        QQmlApplicationEngine* mQmlAppEngine{ nullptr };
    };
}


#endif // XRFIMAGEPROVIDER_H

