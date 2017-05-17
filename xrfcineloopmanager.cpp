#include "xrfcineloop.h"
#include "xrfcineloopmanager.h"

#include <QImage>
#include <QDebug>
#include <QSharedPointer>

namespace xrf {


std::string get_url_no_fragment(const QUrl& loop_url) {
    return loop_url.adjusted(QUrl::RemoveFragment).toString().toStdString();
}

    CineLoopManager::CineLoopManager(QObject *parent) : QObject(parent) {
        mModel = std::make_unique<CineLoopListModel>(this);
    }

    CineLoopListModel *CineLoopManager::model() {
        if(mModel)
            return mModel.get();
        return nullptr;
    }

    void CineLoopManager::addLoopUrl(const QUrl& url_loop) {
        mModel->addLoopUrl(url_loop.adjusted(QUrl::RemoveFragment));
    }

    int CineLoopManager::loopFrameCount(const QUrl& url_loop) const {
        return mModel->loopFrameCount(get_url_no_fragment(url_loop));
    }
    int CineLoopManager::loopCurrentFrameNo(const QUrl& url_loop) const {
        return mModel->loopCurrentFrameNo(get_url_no_fragment(url_loop));
    }
    int CineLoopManager::loopFrameDisplayRate(const QUrl& url_loop) const {
        return mModel->loopFrameDisplayRate(get_url_no_fragment(url_loop));
    }
    QString CineLoopManager::loopDcmTagValuesHtml(const QUrl &url_loop) const {
        return mModel->loopDcmTagValuesHtml(get_url_no_fragment(url_loop));
    }
//    CineLoopRef *CineLoopManager::CineLoop(const QUrl& url_loop) {
//        return mModel->CineLoop(get_url_no_fragment(url_loop));
//    }
    QImage CineLoopManager::GetFrame(const QUrl& url_loop, int frameNo) {
        return mModel->GetFrame(get_url_no_fragment(url_loop), frameNo);
    }
    bool CineLoopManager::contains(const QUrl& url_loop) const {
        return mModel->contains(get_url_no_fragment(url_loop));
    }

}
