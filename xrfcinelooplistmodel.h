#ifndef XRFCINELOOPLISTMODEL_H
#define XRFCINELOOPLISTMODEL_H

#include <QSharedPointer>
#include <QAbstractListModel>

namespace xrf {

class CineLoopRef;
class CineLoopManager;

    class CineLoopListModel : public QAbstractListModel
    {
        Q_OBJECT
    public:
        enum LoopRoles {
            UrlRole = Qt::UserRole + 1,
            FrameCountRole,
        };

        CineLoopListModel(QObject *parent = 0);
        int rowCount(const QModelIndex & parent = QModelIndex()) const;
        QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    public slots:
        void addLoopUrl(const QUrl& loopurl);
        void setManager(CineLoopManager* manager);

    protected:
        QHash<int, QByteArray> roleNames() const;

    private:
        CineLoopManager* mManager{nullptr};
        QHash<QUrl, int> mUrltoIndex;
        QList<QUrl> mLoopUrlList;
    };

}

#endif // XRFCINELOOPLISTMODEL_H
