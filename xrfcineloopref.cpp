#include "xrfcineloopref.h"

namespace xrf {

    CineLoopRef::CineLoopRef(const QUrl loopurl, std::unique_ptr<xrf::CineLoop>&& loop)
        : mUrl(loopurl) {
        mLoop.swap(loop);
    }


    const QUrl CineLoopRef::Url() const {
        return mUrl;
    }

    void CineLoopRef::SetCurrentFrameNo(int frameNo) {
        mCurrentFrmNo = frameNo;
    }

    const int CineLoopRef::GetCurrentFrameNo() const {
        return mCurrentFrmNo;
    }

    const QSharedPointer<QImage> &CineLoopRef::GetFrame(int frameNo) {
        return mLoop->GetFrames().at(frameNo);
    }

    const QSharedPointer<QImage> &CineLoopRef::GetCurrentFrame() {
        return mLoop->GetFrames().at(mCurrentFrmNo);
    }

    const int CineLoopRef::FrameDisplayRate() const {
        return mLoop->GetDcmValues()[RECOMMENDED_DISPLAY_FRAME_RATE].toInt();
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

    const QString CineLoopRef::DcmValuesAsHtml() const {
        return mLoop->GetDcmValuesAsHtml();
    }

}
