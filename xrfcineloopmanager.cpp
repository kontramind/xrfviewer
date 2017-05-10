#include "xrfcineloop.h"
#include "xrfcineloopmanager.h"

#include <QImage>
#include <QDebug>
#include <QSharedPointer>

namespace xrf {

QString get_filepath(const QUrl& loop_url) {
    return loop_url.adjusted(QUrl::PreferLocalFile
                             | QUrl::RemoveAuthority
                             | QUrl::StripTrailingSlash
                             | QUrl::RemoveScheme
                             | QUrl::RemoveFragment
                             | QUrl::NormalizePathSegments)
            .path().remove(0,1);
}

std::string get_url_no_fragment(const QUrl& loop_url) {
    return loop_url.adjusted(QUrl::RemoveFragment).toString().toStdString();
}

CineLoopManager::CineLoopManager(QObject *parent) : QObject(parent) { }

    QUrl CineLoopManager::loopUrl() const {
        return mLoopUrl;
    }

    void CineLoopManager::setLoopUrl(const QUrl url) {
        mLoopUrl = url;
        open_cine_loop();
        read_frame_count();
        read_frame_display_rate();
        read_loop_dcmtagvalues_html();
    }

    CineLoop* CineLoopManager::cineLoop(const QUrl& url_loop) const {
        auto url_no_fragment = get_url_no_fragment(url_loop);
        if(mCineLoopMap.count(url_no_fragment) == 0)
            return nullptr;
        else
            return mCineLoopMap.at(url_no_fragment).get();
    }

    int CineLoopManager::frameCount() const {
        return mFrameCount;
    }

    int CineLoopManager::frameDisplayRate() const {
        return mFrameDisplayRate;
    }

    QString CineLoopManager::loopDcmTagValuesHtml() const {
        return mLoopDcmTagValuesHtml;
    }

    void CineLoopManager::open_cine_loop() {
        auto url_no_fragment = get_url_no_fragment(mLoopUrl);
        if(mCineLoopMap.count(url_no_fragment) == 0)
            mCineLoopMap.insert_or_assign(url_no_fragment, CineLoop::CreatePtr(get_filepath(mLoopUrl)));
        emit loopUrlChanged();
    }

    void CineLoopManager::read_frame_count() {
        auto url_no_fragment = get_url_no_fragment(mLoopUrl);
        mFrameCount = mCineLoopMap.at(url_no_fragment)->GetFrames().size();
        emit frameCountChanged();
    }

    void CineLoopManager::read_frame_display_rate() {
        auto url_no_fragment = get_url_no_fragment(mLoopUrl);
        mFrameCount = mCineLoopMap.at(url_no_fragment)->GetDcmValues()[RECOMMENDED_DISPLAY_FRAME_RATE].toInt();
        emit frameDisplayRateChanged();
    }

    void CineLoopManager::read_loop_dcmtagvalues_html() {
        auto url_no_fragment = get_url_no_fragment(mLoopUrl);
        mLoopDcmTagValuesHtml = mCineLoopMap.at(url_no_fragment)->GetDcmValuesAsHtml();
        emit loopDcmTagValuesHtmlChanged();
    }

}
