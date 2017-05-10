#ifndef XRFCINELOOPMANAGER_H
#define XRFCINELOOPMANAGER_H

#include "xrfcineloop.h"

#include <QUrl>
#include <QObject>

#include <memory>
#include <unordered_map>

namespace xrf {

using CineLoopMap = std::unordered_map<std::string, std::unique_ptr<CineLoop>>;

    class CineLoopManager : public QObject
    {
        Q_OBJECT
        Q_PROPERTY(QUrl loopUrl READ loopUrl WRITE setLoopUrl NOTIFY loopUrlChanged)
        Q_PROPERTY(int frameCount READ frameCount NOTIFY frameCountChanged)
        Q_PROPERTY(int frameDisplayRate READ frameDisplayRate NOTIFY frameDisplayRateChanged)
        Q_PROPERTY(QString loopDcmTagValuesHtml READ loopDcmTagValuesHtml NOTIFY loopDcmTagValuesHtmlChanged)

    public:
        CineLoopManager(QObject *parent = 0);

        QUrl loopUrl() const;
        void setLoopUrl(const QUrl url);

        int frameCount() const;
        int frameDisplayRate() const;
        QString loopDcmTagValuesHtml() const;
        const CineLoop *cineLoop(const QUrl &url_loop) const;

    signals:
        void loopUrlChanged();
        void frameCountChanged();
        void frameDisplayRateChanged();
        void loopDcmTagValuesHtmlChanged();

    public slots:
//        void addLoopUrl(const QUrl& url_loop);

    private:
        void open_cine_loop();
        void read_frame_count();
        void read_frame_display_rate();
        void read_loop_dcmtagvalues_html();

        QUrl mLoopUrl{""};
        int  mFrameCount{0};
        int  mFrameDisplayRate{30};
        QString mLoopDcmTagValuesHtml{""};

        CineLoopMap mCineLoopMap;
    };
}

#endif // XRFCINELOOPMANAGER_H
