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
        //Q_PROPERTY(QUrl loopUrl READ loopUrl WRITE setLoopUrl NOTIFY loopUrlChanged)
        Q_PROPERTY(int frameCount READ frameCount NOTIFY frameCountChanged)
        //Q_PROPERTY(CineLoopListModel model READ model)
        Q_PROPERTY(int frameDisplayRate READ frameDisplayRate NOTIFY frameDisplayRateChanged)
        Q_PROPERTY(QString loopDcmTagValuesHtml READ loopDcmTagValuesHtml NOTIFY loopDcmTagValuesHtmlChanged)

    public:
        CineLoopManager(QObject *parent = 0);

        QUrl loopUrl() const;
        void setLoopUrl(const QUrl& url_Loop);
        const int loopCount() const;

        int frameCount() const;
        int frameDisplayRate() const;
        QString loopDcmTagValuesHtml() const;
        CineLoopRef *CineLoop(const QUrl &url_loop);
        const CineLoopListModel* Model() const { return &mModel;}
//        void SetModel(CineLoopListModel* model) {mModel = model;}
    signals:
        void loopUrlChanged();
        void frameCountChanged();
        void frameDisplayRateChanged();
        void loopDcmTagValuesHtmlChanged();

    public slots:
        void addLoopUrl(const QUrl& url_loop);
        int loopFrameCount(const QUrl& url_loop);
        int loopCurrentFrameNo(const QUrl& url_loop);
        CineLoopListModel* model() { return &mModel;}


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
        CineLoopListModel mModel;
    };
}

#endif // XRFCINELOOPMANAGER_H
