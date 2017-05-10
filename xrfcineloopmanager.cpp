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

    const CineLoop* CineLoopManager::cineLoop(const QUrl& url_loop) const {
        auto url_no_fragment = get_url_no_fragment(url_loop);
        CineLoopMap::const_iterator cit = mCineLoopMap.find(url_no_fragment);
        if(cit == mCineLoopMap.cend())
            return nullptr;
        return cit->second.get();
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
        CineLoopMap::const_iterator cit = mCineLoopMap.find(url_no_fragment);
        if(cit == mCineLoopMap.cend()) {
            // create and insert
            auto xrfLoop = CineLoop::CreatePtr(get_filepath(mLoopUrl));
            auto result =  mCineLoopMap.insert( std::make_pair(url_no_fragment, std::move(xrfLoop)) );
            if(result.second)
                qDebug() << "inserted:" << url_no_fragment.c_str() << ":S_OK";
            else
                qDebug() << "inserted:" << url_no_fragment.c_str() << ":S_FAIL";
        }
        emit loopUrlChanged();
    }

    void CineLoopManager::read_frame_count() {
        mFrameCount = 0;
        auto url_no_fragment = get_url_no_fragment(mLoopUrl);
        CineLoopMap::const_iterator cit = mCineLoopMap.find(url_no_fragment);
        if(cit != mCineLoopMap.cend())
            mFrameCount = cit->second->GetFrames().size();
        emit frameCountChanged();
    }

    void CineLoopManager::read_frame_display_rate() {
        mFrameDisplayRate = 10;
        auto url_no_fragment = get_url_no_fragment(mLoopUrl);
        CineLoopMap::const_iterator cit = mCineLoopMap.find(url_no_fragment);
        if(cit != mCineLoopMap.cend())
            mFrameDisplayRate = cit->second->GetDcmValues()[RECOMMENDED_DISPLAY_FRAME_RATE].toInt();
        emit frameDisplayRateChanged();
    }

    void CineLoopManager::read_loop_dcmtagvalues_html() {
        mLoopDcmTagValuesHtml = "";
        auto url_no_fragment = get_url_no_fragment(mLoopUrl);
        CineLoopMap::const_iterator cit = mCineLoopMap.find(url_no_fragment);
        if(cit != mCineLoopMap.cend())
            mLoopDcmTagValuesHtml = cit->second->GetDcmValuesAsHtml();
        emit loopDcmTagValuesHtmlChanged();
    }

}
