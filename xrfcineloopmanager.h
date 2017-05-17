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

using CineLoopMap = std::unordered_map<std::string, CineLoopRef>;

    class CineLoopManager : public QObject
    {
        Q_OBJECT
    public:
        CineLoopManager(QObject *parent = 0);
        CineLoopRef *CineLoop(const QUrl &url_loop);

    public slots:
        void addLoopUrl(const QUrl& url_loop);
        int loopFrameCount(const QUrl& url_loop);
        int loopCurrentFrameNo(const QUrl& url_loop);
        int loopFrameDisplayRate(const QUrl& url_loop);
        QString loopDcmTagValuesHtml(const QUrl& url_loop);
        CineLoopListModel* model();


    private:
        void open_cine_loop(const QUrl& url_loop);

        CineLoopMap mCineLoopMap;
        std::unique_ptr<CineLoopListModel> mModel{nullptr};
    };
}

#endif // XRFCINELOOPMANAGER_H
