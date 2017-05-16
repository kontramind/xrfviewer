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

    CineLoopManager::CineLoopManager(QObject *parent)
        : QObject(parent) {
        mModel.setManager(this);
    }

    QUrl CineLoopManager::loopUrl() const {
        return mLoopUrl;
    }

    void CineLoopManager::setLoopUrl(const QUrl& url_loop) {
        mLoopUrl = url_loop;
        open_cine_loop();
        read_frame_count();
        read_frame_display_rate();
        read_loop_dcmtagvalues_html();
    }

    const int CineLoopManager::loopCount() const {
        return mCineLoopMap.size();
    }

    void CineLoopManager::addLoopUrl(const QUrl& url_loop) {
        mLoopUrl = url_loop;
        open_cine_loop();
        read_frame_count();
        read_frame_display_rate();
        read_loop_dcmtagvalues_html();
        mModel.addLoopUrl(mLoopUrl);
    }

    int CineLoopManager::loopFrameCount(const QUrl& url_loop) {
        auto url_no_fragment = get_url_no_fragment(url_loop);
        CineLoopMap::const_iterator cit = mCineLoopMap.find(url_no_fragment);
        if(cit == mCineLoopMap.cend())
            return 0;
        return cit->second.FrameCount();
    }

    int CineLoopManager::loopCurrentFrameNo(const QUrl& url_loop) {
        auto url_no_fragment = get_url_no_fragment(url_loop);
        CineLoopMap::const_iterator cit = mCineLoopMap.find(url_no_fragment);
        if(cit == mCineLoopMap.cend())
            return 0;
        return cit->second.CurrentFrameNo();
    }

    CineLoopRef *CineLoopManager::CineLoop(const QUrl& url_loop) {
        auto url_no_fragment = get_url_no_fragment(url_loop);
        CineLoopMap::iterator cit = mCineLoopMap.find(url_no_fragment);
        if(cit == mCineLoopMap.cend())
            return nullptr;
        return &cit->second; //.CineLoop();// get();
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
            auto result =  mCineLoopMap.insert(std::make_pair(url_no_fragment, CineLoopRef(mLoopUrl, std::move(CineLoop::CreatePtr(get_filepath(mLoopUrl))))) );
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
            mFrameCount = cit->second.FrameCount();
        emit frameCountChanged();
    }

    void CineLoopManager::read_frame_display_rate() {
        mFrameDisplayRate = 10;
        auto url_no_fragment = get_url_no_fragment(mLoopUrl);
        CineLoopMap::const_iterator cit = mCineLoopMap.find(url_no_fragment);
        if(cit != mCineLoopMap.cend())
            mFrameDisplayRate = cit->second.GetDcmValues()[RECOMMENDED_DISPLAY_FRAME_RATE].toInt();
        emit frameDisplayRateChanged();
    }

    void CineLoopManager::read_loop_dcmtagvalues_html() {
        mLoopDcmTagValuesHtml = "";
        auto url_no_fragment = get_url_no_fragment(mLoopUrl);
        CineLoopMap::const_iterator cit = mCineLoopMap.find(url_no_fragment);
        if(cit != mCineLoopMap.cend())
            mLoopDcmTagValuesHtml = cit->second.GetDcmValuesAsHtml();
        emit loopDcmTagValuesHtmlChanged();
    }

}
