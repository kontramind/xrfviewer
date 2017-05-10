#include "xrfimageprovider.h"
#include "xrfcineloop.h"

#include <QDebug>
#include <QQmlApplicationEngine>

namespace xrf {

QString get_file_path(const QUrl& loop_url) {
    return loop_url.adjusted(QUrl::PreferLocalFile
                             | QUrl::RemoveAuthority
                             | QUrl::StripTrailingSlash
                             | QUrl::RemoveScheme
                             | QUrl::RemoveFragment
                             | QUrl::NormalizePathSegments)
            .path().remove(0,1);
}

int get_frame_no(const QUrl& loop_url) {
    if(loop_url.isEmpty() || loop_url.fragment().isNull())
        return 0;
    return loop_url.fragment().toInt();
}

    ImageProvider::ImageProvider(QQmlApplicationEngine* qmlAppEngine, CineLoopManager *manager)
        : QQuickImageProvider(QQuickImageProvider::Image), mQmlAppEngine(qmlAppEngine), mCineLoopManager(manager) { }

    QImage ImageProvider::requestImage(const QString &url_loop, QSize *size, const QSize &requestedSize)
    {
//        Q_UNUSED(size);
//        Q_UNUSED(requestedSize);

        QImage xrfFrame(":/icons/no_signal.png");
        if(!mCineLoopManager)
            return xrfFrame;
        auto xrfLoop = mCineLoopManager->cineLoop(url_loop);
        if(!xrfLoop)
            return xrfFrame;

        xrfFrame = QImage(*mCineLoopManager->cineLoop(url_loop)->GetFrames()[get_frame_no({url_loop})]);
        return xrfFrame;
    }

}
