#include "xrfcineloopref.h"

namespace xrf {

    CineLoopRef::CineLoopRef(const QUrl loopurl, std::unique_ptr<xrf::CineLoop>&& loop)
        : mUrl(loopurl) {
        mLoop.swap(loop);
    }

    const QSharedPointer<QImage> &CineLoopRef::GetFrame(int frmNo) {
        mCurrentFrmNo = frmNo;
        return mLoop->GetFrames().at(mCurrentFrmNo);
    }

    const QUrl CineLoopRef::Url() const {
        return mUrl;
    }

    const int CineLoopRef::CurrentFrameNo() const {
        return mCurrentFrmNo;
    }

    const int CineLoopRef::FrameCount() const {
        return mLoop->GetFrames().size();
    }

    const CineLoop *CineLoopRef::CineLoop() const {
        return mLoop.get();
    }

    const DcmTagValues& CineLoopRef::GetDcmValues() const {
        return mLoop->GetDcmValues();
    }

    const QString CineLoopRef::GetDcmValuesAsHtml() const {
        return mLoop->GetDcmValuesAsHtml();
    }

}
