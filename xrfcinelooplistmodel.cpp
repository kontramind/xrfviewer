#include "xrfcinelooplistmodel.h"
#include "xrfcineloopmanager.h"

#include <QDebug>

namespace xrf {

    CineLoopListModel::CineLoopListModel(CineLoopManager *manager, QObject *parent)
        : QAbstractListModel(parent), mManager(manager) {}

    void CineLoopListModel::addLoopUrl(const QUrl& loopurl) {
        if(mUrltoIndex.contains(loopurl))
            return;
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        mLoopUrlList << loopurl;
        mUrltoIndex[loopurl] = rowCount()-1;
        endInsertRows();
    }

    int CineLoopListModel::rowCount(const QModelIndex & parent) const {
        //Q_UNUSED(parent);
        auto rows = mLoopUrlList.size();
        return rows;
    }

    QHash<int, QByteArray> CineLoopListModel::roleNames() const {
        QHash<int, QByteArray> roles;
        roles[UrlRole] = "url";
        roles[CurrentFrameNoRole] = "currentframeno";
        roles[FrameCountRole] = "framecount";
        return roles;
    }

    QVariant CineLoopListModel::data(const QModelIndex & index, int role) const {
        if (index.row() < 0 || index.row() >= mLoopUrlList.count())
            return QVariant();
        if(!index.isValid())
            return QVariant();
        if(mLoopUrlList.count() == 0)
            return QVariant();

        auto url = mLoopUrlList[index.row()];
        if (role == UrlRole) {
            return url;
        } else if (role == FrameCountRole) {
            return mManager->CineLoop(url)->FrameCount();
        } else if(role == CurrentFrameNoRole) {
            return mManager->loopCurrentFrameNo(url);
        }
        return QVariant();
    }
}

