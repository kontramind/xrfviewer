#ifndef XRFIMAGEPROVIDER_H
#define XRFIMAGEPROVIDER_H

#include <QFont>
#include <QImage>
#include <QPixmap>
#include <QString>
#include <QPainter>
#include <QQuickImageProvider>

#include <memory>

class QQmlApplicationEngine;

namespace xrf {

    class CineLoop;

    class XRFImageProvider : public QQuickImageProvider
    {
    public:
        XRFImageProvider(QQmlApplicationEngine* qmlAppEngine);
        QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);
        const int getFrameCount() const;
        const int getCurrFrameNo() const;
        const int getFrameRecommendedDisplayRate() const;
        const QString getFrameDcmKeyTagValuesAsHtml() const;

    protected:
        void forwardCurrentFrameNo() const;
        void forwardFrameDcmKeyTagValues() const;

    private:
        QUrl mUrl {""};
        int mCurrFrameNo{-1};
        QImage mCurrFrame{QImage()};
        std::unique_ptr<CineLoop> mCineLoop{ nullptr };
        QQmlApplicationEngine* mQmlAppEngine{ nullptr };
    };

}


#endif // XRFIMAGEPROVIDER_H

