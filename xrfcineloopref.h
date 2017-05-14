#ifndef XRFCINELOOPREF_H
#define XRFCINELOOPREF_H

#include "xrfcineloop.h"
#include <QSharedPointer>

namespace xrf {
class CineLoopRef {
public:
    CineLoopRef(std::unique_ptr<xrf::CineLoop>&& loop);
    const QSharedPointer<QImage>& GetFrame(int frmNo);

    const int FrameCount() const;
    const int CurrentFrameNo() const;
    const xrf::CineLoop* CineLoop() const;
    const DcmTagValues& GetDcmValues() const;
    const QString GetDcmValuesAsHtml() const;
private:
    int mCurrentFrmNo{0};
    std::unique_ptr<xrf::CineLoop> mLoop;
};
}
#endif // XRFCINELOOPREF_H
