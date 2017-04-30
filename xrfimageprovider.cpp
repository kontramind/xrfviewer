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
QString get_frame_direction(const QUrl& loop_url) {
    return loop_url.fragment();
}

    XRFImageProvider::XRFImageProvider(QQmlApplicationEngine* qmlAppEngine)
        : QQuickImageProvider(QQuickImageProvider::Image),
          mQmlAppEngine(qmlAppEngine), mCurrFrame {QImage(":/icons/no_signal.png")} { }

    QImage XRFImageProvider::requestImage(const QString &loopid, QSize *size, const QSize &requestedSize)
    {
        if(get_file_path({loopid}) != get_file_path(mUrl))
            mCineLoop.reset(nullptr);

        mUrl = {loopid};
        auto file_path = get_file_path(mUrl);
        auto frame_direction = get_frame_direction(mUrl);
        if(!file_path.isNull() && !file_path.isEmpty()) {
            if(!mCineLoop) {
                mCurrFrameNo = -1;
                mCineLoop = CineLoop::CreatePtr(file_path);
                if(mCineLoop->IsValid())
                    mCurrFrameNo = 0;
            }
            if(mCineLoop->IsValid()) {
                if(frame_direction == "next")
                    mCurrFrameNo = ++mCurrFrameNo >= getFrameCount() ? 0 : mCurrFrameNo;
                else if(frame_direction == "prev")
                    mCurrFrameNo = --mCurrFrameNo < 0 ? getFrameCount() -1 : mCurrFrameNo;
                forwardCurrentFrameNo();
                return QImage(*mCineLoop->GetFrames()[mCurrFrameNo]);
            }
        }
        forwardCurrentFrameNo();
        return QImage(":/icons/no_signal.png");
    }

    const int XRFImageProvider::getFrameCount() const
    {
        if(mCineLoop && mCineLoop->IsValid())
            return mCineLoop->GetFrames().size();
        else
            return 0;
    }

    const int XRFImageProvider::getCurrFrameNo() const
    {
        return mCurrFrameNo;
    }

    const int XRFImageProvider::getFrameRecommendedDisplayRate() const
    {
        if(mCineLoop && mCineLoop->IsValid())
            return mCineLoop->GetDcmValues().contains(RECOMMENDED_DISPLAY_FRAME_RATE) ?
                        mCineLoop->GetDcmValues()[RECOMMENDED_DISPLAY_FRAME_RATE].toInt() : 10;
    }

    void XRFImageProvider::forwardCurrentFrameNo() const
    {
        if(mQmlAppEngine) {
            if(!mQmlAppEngine->rootObjects().empty()) {
                auto rootObject = mQmlAppEngine->rootObjects().first();
                rootObject->setProperty("frameCurrentNo", mCurrFrameNo);
                rootObject->setProperty("frameTotalCount", getFrameCount());
                rootObject->setProperty("frameRecommendedDisplayRate", getFrameRecommendedDisplayRate());
            }
        }
    }

}
