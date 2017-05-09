#ifndef XRFCINELOOPMANAGER_H
#define XRFCINELOOPMANAGER_H

#include "xrfcineloop.h"

#include <QUrl>
#include <QObject>

#include <memory>

namespace xrf {

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

        CineLoop* cineLoop() const;
        int frameCount() const;
        int frameDisplayRate() const;
        QString loopDcmTagValuesHtml() const;

    signals:
        void loopUrlChanged();
        void frameCountChanged();
        void frameDisplayRateChanged();
        void loopDcmTagValuesHtmlChanged();

    public slots:

    private:
        void open_cine_loop();
        void read_frame_count();
        void read_frame_display_rate();
        void read_loop_dcmtagvalues_html();

        QUrl mLoopUrl{""};
        int  mFrameCount{0};
        int  mFrameDisplayRate{30};
        QString mLoopDcmTagValuesHtml{""};
        std::unique_ptr<CineLoop> mCineLoop{ nullptr };
        //QHash<QUrl, std::unique_ptr<CineLoop>> mCineLoops;
    };
}

#endif // XRFCINELOOPMANAGER_H
