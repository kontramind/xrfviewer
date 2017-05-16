#include "xrfcinelooplistmodel.h"
#include "xrfcineloopmanager.h"

#include <QDebug>

namespace xrf {

    CineLoopListModel::CineLoopListModel(QObject *parent)
        : QAbstractListModel(parent) {}

    void CineLoopListModel::setManager(CineLoopManager *manager) {
        mManager = manager;
    }

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

        if (role == UrlRole) {
            return mLoopUrlList[index.row()];
        } else if (role == FrameCountRole) {
            auto url = mLoopUrlList[index.row()];
            return mManager->CineLoop(url)->FrameCount();
        }
        return QVariant();
    }
}

