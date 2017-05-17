#include "xrfcinelooplistmodel.h"
#include "xrfcineloopmanager.h"
#include "xrfcineloopref.h"

#include <QDebug>

namespace xrf {

static QString  get_filepath(const QUrl& loop_url) {
    return loop_url.adjusted(QUrl::PreferLocalFile
                             | QUrl::RemoveAuthority
                             | QUrl::StripTrailingSlash
                             | QUrl::RemoveScheme
                             | QUrl::RemoveFragment
                             | QUrl::NormalizePathSegments)
            .path().remove(0,1);
}

    CineLoopListModel::CineLoopListModel(CineLoopManager *manager, QObject *parent)
        : QAbstractListModel(parent), mManager(manager) {}

    void CineLoopListModel::addLoopUrl(const QUrl& url_loop) {
        if(mUrltoIndex.contains(url_loop))
            return;
        beginInsertRows(QModelIndex(), rowCount(), rowCount());
        open_cine_loop(url_loop);
        mLoopUrlList << url_loop;
        mUrltoIndex[url_loop] = rowCount()-1;
        endInsertRows();
    }
    void CineLoopListModel::open_cine_loop(const QUrl& url_loop) {
        CineLoopMap::const_iterator cit = mCineLoopMap.find(url_loop.toString().toStdString());
        if(cit == mCineLoopMap.cend()) {
            // create and insert
            std::string looppath = url_loop.toString().toStdString();
            auto result = mCineLoopMap.insert(std::make_pair(looppath, CineLoopRef(url_loop, std::move(CineLoop::CreatePtr(get_filepath(url_loop))))) );
            if(result.second)
                qDebug() << "inserted:" << looppath.c_str() << ":S_OK";
            else
                qDebug() << "inserted:" << looppath.c_str() << ":S_FAIL";
        }
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
            return loopFrameCount(url.toString().toStdString());
        } else if(role == CurrentFrameNoRole) {
            return loopCurrentFrameNo(url.toString().toStdString());
        }
        return QVariant();
    }

    CineLoopRef *CineLoopListModel::CineLoop(const std::string& url_loop) {
        CineLoopMap::iterator cit = mCineLoopMap.find(url_loop);
        if(cit == mCineLoopMap.cend())
            return nullptr;
        return &cit->second;
    }

    int CineLoopListModel::loopFrameCount(const std::string& url_loop) const {
        CineLoopMap::const_iterator cit = mCineLoopMap.find(url_loop);
        if(cit == mCineLoopMap.cend())
            return 0;
        return cit->second.FrameCount();
    }
    int CineLoopListModel::loopCurrentFrameNo(const std::string& url_loop) const {
        CineLoopMap::const_iterator cit = mCineLoopMap.find(url_loop);
        if(cit == mCineLoopMap.cend())
            return 0;
        return cit->second.CurrentFrameNo();
    }
    int CineLoopListModel::loopFrameDisplayRate(const std::string& url_loop) const {
        CineLoopMap::const_iterator cit = mCineLoopMap.find(url_loop);
        if(cit == mCineLoopMap.cend())
            return 1;
        return cit->second.FrameDisplayRate();
    }
    QString CineLoopListModel::loopDcmTagValuesHtml(const std::string &url_loop) const {
        CineLoopMap::const_iterator cit = mCineLoopMap.find(url_loop);
        if(cit != mCineLoopMap.cend())
            return cit->second.DcmValuesAsHtml();
        return {};
    }
    bool CineLoopListModel::contains(const std::string& url_loop) const {
        CineLoopMap::const_iterator cit = mCineLoopMap.find(url_loop);
        return cit != mCineLoopMap.cend();
    }
}

