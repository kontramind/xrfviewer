#ifndef XRFCINELOOPREF_H
#define XRFCINELOOPREF_H

#include "xrfcineloop.h"
#include <QUrl>
#include <QSharedPointer>

namespace xrf {
class CineLoopRef {
public:
    CineLoopRef(const QUrl loopurl, std::unique_ptr<xrf::CineLoop>&& loop);

    const QUrl Url() const;
    const int FrameCount() const;
    const int CurrentFrameNo() const;
    const int FrameDisplayRate() const;
    const xrf::CineLoop* CineLoop() const;
    const DcmTagValues& GetDcmValues() const;
    const QString GetDcmValuesAsHtml() const;
    const QSharedPointer<QImage>& GetFrame(int frmNo);
private:
    QUrl mUrl{""};
    int mCurrentFrmNo{0};
    std::unique_ptr<xrf::CineLoop> mLoop{nullptr};
};
}
#endif // XRFCINELOOPREF_H
