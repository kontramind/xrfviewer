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
        mModel = std::make_unique<CineLoopListModel>(this);
    }

    QUrl CineLoopManager::loopUrl() const {
        return mLoopUrl;
    }

    void CineLoopManager::setLoopUrl(const QUrl& url_loop) {
        mLoopUrl = url_loop;
        open_cine_loop();
//        read_loop_dcmtagvalues_html();
    }

    const int CineLoopManager::loopCount() const {
        return mCineLoopMap.size();
    }

    void CineLoopManager::addLoopUrl(const QUrl& url_loop) {
        mLoopUrl = url_loop;
        open_cine_loop();
//        read_loop_dcmtagvalues_html();
        mModel->addLoopUrl(mLoopUrl);
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

    CineLoopListModel *CineLoopManager::model() {
        if(mModel)
            return mModel.get();
        else
            return nullptr;
    }

    CineLoopRef *CineLoopManager::CineLoop(const QUrl& url_loop) {
        auto url_no_fragment = get_url_no_fragment(url_loop);
        CineLoopMap::iterator cit = mCineLoopMap.find(url_no_fragment);
        if(cit == mCineLoopMap.cend())
            return nullptr;
        return &cit->second;
    }

    int CineLoopManager::loopFrameDisplayRate(const QUrl& url_loop) {
        auto url_no_fragment = get_url_no_fragment(url_loop);
        CineLoopMap::const_iterator cit = mCineLoopMap.find(url_no_fragment);
        if(cit == mCineLoopMap.cend())
            return 1;
        return cit->second.FrameDisplayRate();
    }

    QString CineLoopManager::loopDcmTagValuesHtml(const QUrl &url_loop) {
        auto url_no_fragment = get_url_no_fragment(url_loop);
        CineLoopMap::const_iterator cit = mCineLoopMap.find(url_no_fragment);
        if(cit != mCineLoopMap.cend())
            return cit->second.DcmValuesAsHtml();
        return {};
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

//    void CineLoopManager::read_loop_dcmtagvalues_html() {
//        mLoopDcmTagValuesHtml = "";
//        auto url_no_fragment = get_url_no_fragment(mLoopUrl);
//        CineLoopMap::const_iterator cit = mCineLoopMap.find(url_no_fragment);
//        if(cit != mCineLoopMap.cend())
//            mLoopDcmTagValuesHtml = cit->second.GetDcmValuesAsHtml();
//        emit loopDcmTagValuesHtmlChanged();
//    }

}
