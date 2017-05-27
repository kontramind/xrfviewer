#ifndef XRFCINELOOPREF_H
#define XRFCINELOOPREF_H

#include "xrfcineloop.h"
#include <QUrl>
#include <QSharedPointer>

namespace xrf {
class CineLoopRef {
public:
    CineLoopRef(const QUrl loopurl, std::unique_ptr<xrf::CineLoop>&& loop);

    void SetCurrentFrameNo(int frameNo);
    const int GetCurrentFrameNo() const;

    const QUrl Url() const;
    const int CineRate() const;
    const int FrameCount() const;
    const int FrameDisplayRate() const;
    const DcmTagValues& GetDcmValues() const;
    const QString DcmValuesAsHtml() const;
    const QSharedPointer<QImage>& GetCurrentFrame();
    const QSharedPointer<QImage>& GetFrame(int frmNo);
private:
    const xrf::CineLoop* CineLoop() const;

    QUrl mUrl{""};
    int mCurrentFrmNo{0};
    std::unique_ptr<xrf::CineLoop> mLoop{nullptr};
};
}
#endif // XRFCINELOOPREF_H
