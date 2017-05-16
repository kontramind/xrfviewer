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
        Q_PROPERTY(QString loopDcmTagValuesHtml READ loopDcmTagValuesHtml NOTIFY loopDcmTagValuesHtmlChanged)

    public:
        CineLoopManager(QObject *parent = 0);

        QUrl loopUrl() const;
        void setLoopUrl(const QUrl& url_Loop);
        const int loopCount() const;

        QString loopDcmTagValuesHtml() const;
        CineLoopRef *CineLoop(const QUrl &url_loop);

    signals:
        void loopUrlChanged();
        void loopDcmTagValuesHtmlChanged();

    public slots:
        void addLoopUrl(const QUrl& url_loop);
        int loopFrameCount(const QUrl& url_loop);
        int loopCurrentFrameNo(const QUrl& url_loop);
        int loopFrameDisplayRate(const QUrl& url_loop);
        CineLoopListModel* model();


    private:
        void open_cine_loop();
        void read_loop_dcmtagvalues_html();

        QUrl mLoopUrl{""};
        QString mLoopDcmTagValuesHtml{""};

        CineLoopMap mCineLoopMap;
        std::unique_ptr<CineLoopListModel> mModel{nullptr};
    };
}

#endif // XRFCINELOOPMANAGER_H
