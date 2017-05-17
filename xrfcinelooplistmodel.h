#ifndef XRFCINELOOPLISTMODEL_H
#define XRFCINELOOPLISTMODEL_H

#include <QSharedPointer>
#include <QAbstractListModel>

#include <unordered_map>

namespace xrf {

class CineLoopRef;
class CineLoopManager;
using CineLoopMap = std::unordered_map<std::string, CineLoopRef>;

    class CineLoopListModel : public QAbstractListModel
    {
        Q_OBJECT
    public:
        enum LoopRoles {
            UrlRole = Qt::UserRole + 1,
            CurrentFrameNoRole,
            FrameCountRole,
        };

        CineLoopListModel(CineLoopManager* manager, QObject *parent = 0);
        int rowCount(const QModelIndex & parent = QModelIndex()) const;
        QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    public slots:
        void addLoopUrl(const QUrl& url_loop);
        CineLoopRef *CineLoop(const std::string &url_loop);
        int loopFrameCount(const std::string &url_loop);
        int loopCurrentFrameNo(const std::string &url_loop);
        int loopFrameDisplayRate(const std::string &url_loop);
        QString loopDcmTagValuesHtml(const std::string &url_loop);

    protected:
        QHash<int, QByteArray> roleNames() const;    
        void open_cine_loop(const QUrl &url_loop);

    private:
        QList<QUrl> mLoopUrlList;
        CineLoopMap mCineLoopMap;
        QHash<QUrl, int> mUrltoIndex;
        CineLoopManager* mManager{nullptr};
    };

}

#endif // XRFCINELOOPLISTMODEL_H
