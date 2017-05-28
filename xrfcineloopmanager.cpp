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
        mModelRcv = std::make_unique<CineLoopListModel>(this);
    }

    CineLoopListModel *CineLoopManager::model() {
        if(mModel)
            return mModel.get();
        return nullptr;
    }
    CineLoopListModel *CineLoopManager::modelrcv() {
        if(mModelRcv)
            return mModelRcv.get();
        return nullptr;
    }

    void CineLoopManager::addLoopUrl(const QUrl& url_loop) {
        mModel->addLoopUrl(url_loop.adjusted(QUrl::RemoveFragment));
    }
    void CineLoopManager::addLoopRcv(const QUrl& url_loop) {
        mModelRcv->addLoopUrl(url_loop.adjusted(QUrl::RemoveFragment));
    }
    void CineLoopManager::dropAllLoopRcv() {
        mModelRcv->dropAllLoopUrl();
    }

    int CineLoopManager::loopFrameCount(const QUrl& url_loop) const {
        auto count = mModel->loopFrameCount(get_url_no_fragment(url_loop));
        return count;
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
    void CineLoopManager::setCurrentFrameNo(const QUrl& url_loop, const int frameNo) {
        mModel->SetCurrentFrameNo(get_url_no_fragment(url_loop), frameNo);
        emit dataChanged();
    }
    QImage CineLoopManager::GetCurrentFrame(const QUrl& url_loop) {
        return mModel->GetCurrentFrame(get_url_no_fragment(url_loop));
    }
    void CineLoopManager::incrementCurrentFrameNoForAllLoops() {
        mModel->IncrementCurrentFrameNoForAllLoops();
    }
    QImage CineLoopManager::GetFrame(const QUrl& url_loop, int frameNo) {
        return mModel->GetFrame(get_url_no_fragment(url_loop), frameNo);
    }

    int CineLoopManager::getModelIndex(const QUrl& url_loop) {
        return mModel->GetModelIndex(url_loop);
    }

    bool CineLoopManager::contains(const QUrl& url_loop) const {
        return mModel->contains(get_url_no_fragment(url_loop));
    }

}
