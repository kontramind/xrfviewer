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

class ImageProvider;

class CineLoopListModel;

    class CineLoopManager : public QObject
    {
        Q_OBJECT
    public:
        friend class ImageProvider;
        CineLoopManager(QObject *parent = 0);

        CineLoopListModel* model();
        Q_INVOKABLE void addLoopUrl(const QUrl& url_loop);
        Q_INVOKABLE bool contains(const QUrl &url_loop) const;
        Q_INVOKABLE int loopFrameCount(const QUrl& url_loop) const;
        Q_INVOKABLE int loopCurrentFrameNo(const QUrl& url_loop) const;
        Q_INVOKABLE int loopFrameDisplayRate(const QUrl& url_loop) const;
        Q_INVOKABLE QString loopDcmTagValuesHtml(const QUrl& url_loop) const;

        Q_INVOKABLE void setCurrentFrameNo(const QUrl& url_loop, const int frameNo);
        QImage GetCurrentFrame(const QUrl& url_loop);

        Q_INVOKABLE void incrementCurrentFrameNoForAllLoops();
        Q_INVOKABLE int getModelIndex(const QUrl& url_loop);
    public slots:

    signals:
        // emited whenever
        // state becomes dirty
        void dataChanged() const;

    private:
        QImage GetFrame(const QUrl& url_loop, int frameNo);

        std::unique_ptr<CineLoopListModel> mModel{nullptr};
    };
}

#endif // XRFCINELOOPMANAGER_H
