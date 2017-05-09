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
    if(loop_url.fragment().isNull() || loop_url.isEmpty())
        return 0;
    return loop_url.fragment().toInt();
}

    ImageProvider::ImageProvider(QQmlApplicationEngine* qmlAppEngine, CineLoopManager *manager)
        : QQuickImageProvider(QQuickImageProvider::Image), mQmlAppEngine(qmlAppEngine), mCineLoopManager(manager) { }

    QImage ImageProvider::requestImage(const QString &loopid, QSize *size, const QSize &requestedSize)
    {
//        Q_UNUSED(size);
//        Q_UNUSED(requestedSize);

        QImage xrfFrame(":/icons/no_signal.png");
        if(!mCineLoopManager || !mCineLoopManager->cineLoop())
            return xrfFrame;

        xrfFrame = QImage(*mCineLoopManager->cineLoop()->GetFrames()[get_frame_no({loopid})]);
//        if(get_file_path({loopid}) != get_file_path(mUrl))
//            mCineLoop.reset(nullptr);

//        mUrl = {loopid};
//        auto file_path = get_file_path(mUrl);
//        auto frame_no = get_frame_direction(mUrl);
//        if(!file_path.isNull() && !file_path.isEmpty()) {
//            if(!mCineLoop)
//                mCineLoop = CineLoop::CreatePtr(file_path);
//            if(mCineLoop->IsValid()) {
//                mCurrFrameNo = frame_no.toUInt();
//                xrfFrame = QImage(*mCineLoop->GetFrames()[mCurrFrameNo]);
//            }
//        }
//        if (requestedSize.isValid())
//            xrfFrame = xrfFrame.scaled(requestedSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
//        *size = xrfFrame.size();
        return xrfFrame;
    }

//    void ImageProvider::setCineLoop(CineLoop* loop) {
//        mCineLoop = loop;
//    }

//    const int ImageProvider::getFrameCount() const
//    {
//        if(mCineLoop && mCineLoop->IsValid())
//            return mCineLoop->GetFrames().size();
//        else
//            return 0;
//    }

    const QString ImageProvider::getFrameDcmKeyTagValuesAsHtml() const {
        if(!mCineLoop || !mCineLoop->IsValid())
            return {""};
        return mCineLoop->GetDcmValuesAsHtml();
    }

}
