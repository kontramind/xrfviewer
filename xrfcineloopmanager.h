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
        QImage GetFrame(const QUrl& url_loop, int frameNo);

        Q_INVOKABLE bool contains(const QUrl &url_loop) const;
        Q_INVOKABLE int loopFrameCount(const QUrl& url_loop) const;
        Q_INVOKABLE int loopCurrentFrameNo(const QUrl& url_loop) const;
    public slots:
        CineLoopListModel* model();
        void addLoopUrl(const QUrl& url_loop);



        int loopFrameDisplayRate(const QUrl& url_loop) const;
        QString loopDcmTagValuesHtml(const QUrl& url_loop) const;

    signals:
        void dataChanged() const;

    private:
        std::unique_ptr<CineLoopListModel> mModel{nullptr};
    };
}

#endif // XRFCINELOOPMANAGER_H
