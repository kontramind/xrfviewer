#ifndef XRFCINELOOPMANAGER_H
#define XRFCINELOOPMANAGER_H

#include "xrfcineloopref.h"
#include "xrfcinelooplistmodel.h"

#include <QUrl>
#include <QImage>
#include <QObject>

#include <memory>
#include <unordered_map>

namespace xrf {

class CineLoopListModel;

    class CineLoopManager : public QObject
    {
        Q_OBJECT
    public:
        CineLoopManager(QObject *parent = 0);
//        CineLoopRef *CineLoop(const QUrl &url_loop);
        QImage GetFrame(const QUrl& url_loop, int frameNo);

    public slots:
        CineLoopListModel* model();
        void addLoopUrl(const QUrl& url_loop);
        bool contains(const QUrl &url_loop) const;
        int loopFrameCount(const QUrl& url_loop) const;
        int loopCurrentFrameNo(const QUrl& url_loop) const;
        int loopFrameDisplayRate(const QUrl& url_loop) const;
        QString loopDcmTagValuesHtml(const QUrl& url_loop) const;

    private:
        std::unique_ptr<CineLoopListModel> mModel{nullptr};
    };
}

#endif // XRFCINELOOPMANAGER_H
