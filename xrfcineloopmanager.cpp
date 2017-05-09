#include "xrfcineloop.h"
#include "xrfcineloopmanager.h"

#include <QImage>
#include <QDebug>
#include <QSharedPointer>

namespace xrf {

QString get_filepath(const QUrl& loop_url) {
    return loop_url.adjusted(QUrl::PreferLocalFile
                             | QUrl::RemoveAuthority
                             | QUrl::StripTrailingSlash
                             | QUrl::RemoveScheme
                             | QUrl::RemoveFragment
                             | QUrl::NormalizePathSegments)
            .path().remove(0,1);
}

QUrl get_url_no_fragment(const QUrl& loop_url) {
    return loop_url.adjusted(QUrl::RemoveFragment);
}

CineLoopManager::CineLoopManager(QObject *parent) : QObject(parent) { }

    QUrl CineLoopManager::loopUrl() const {
        return mLoopUrl;
    }

    void CineLoopManager::setLoopUrl(const QUrl url) {
        mLoopUrl = url;
        open_cine_loop();
        read_frame_count();
        read_frame_display_rate();
        read_loop_dcmtagvalues_html();
    }

    CineLoop* CineLoopManager::cineLoop() const {
        return mCineLoop.get();
    }

    int CineLoopManager::frameCount() const {
        return mFrameCount;
    }

    int CineLoopManager::frameDisplayRate() const {
        return mFrameDisplayRate;
    }

    QString CineLoopManager::loopDcmTagValuesHtml() const {
        return mLoopDcmTagValuesHtml;
    }

    void CineLoopManager::open_cine_loop() {
        mCineLoop.reset(nullptr);
        mCineLoop = CineLoop::CreatePtr(get_filepath(mLoopUrl));
        emit loopUrlChanged();
    }

    void CineLoopManager::read_frame_count() {
        if(mCineLoop && mCineLoop->IsValid())
            mFrameCount = mCineLoop->GetFrames().size();
        emit frameCountChanged();
    }

    void CineLoopManager::read_frame_display_rate() {
        if(mCineLoop && mCineLoop->IsValid())
            mFrameDisplayRate = mCineLoop->GetDcmValues().contains(RECOMMENDED_DISPLAY_FRAME_RATE) ?
                                mCineLoop->GetDcmValues()[RECOMMENDED_DISPLAY_FRAME_RATE].toInt() : 30;
        emit frameDisplayRateChanged();
    }

    void CineLoopManager::read_loop_dcmtagvalues_html() {
        mLoopDcmTagValuesHtml = {""};
        if(mCineLoop && mCineLoop->IsValid())
            mLoopDcmTagValuesHtml = mCineLoop->GetDcmValuesAsHtml();
        emit loopDcmTagValuesHtmlChanged();
    }

}


//QImage processaImagem::carregaImagem()
//{
//    QUrl caminhoImagem(p_caminhoImagem);
//    QQmlEngine *engine = QQmlEngine::contextForObject(this)->engine();
//    QQmlImageProviderBase *imageProviderBase = engine->imageProvider(caminhoImagem.host());
//    QQuickImageProvider *imageProvider = static_cast<QQuickImageProvider*>(imageProviderBase);


//    QSize imageSize;
//    QString imageId = caminhoImagem.path().remove(0, 1);
//    QImage imagem = imageProvider->requestImage(imageId, &imageSize, imageSize);

//    if(imagem.isNull())
//    {
//        qDebug() << "Erro ao carregar a imagem";
//        imagem = QImage();
//    }
//    else
//    {
//        if((p_anguloOrientacaoCamera == 90) || (p_anguloOrientacaoCamera == 270))
//        {
//            int larguraImagem = p_tamanhoImagem.width();
//            int alturaImagem = p_tamanhoImagem.height();

//            p_tamanhoImagem.setWidth(alturaImagem);
//            p_tamanhoImagem.setHeight(larguraImagem);

//            int recorteX = p_rectRecorte.x();
//            int recorteY = p_rectRecorte.y();
//            int recorteLargura = p_rectRecorte.width();
//            int recorteAltura = p_rectRecorte.height();

//            p_rectRecorte.setRect(recorteY, recorteX, recorteAltura, recorteLargura);

//            if(imagem.size().width() > imagem.size().height())
//            {
//                QTransform rotacao;
//                rotacao.rotate(360 - p_anguloOrientacaoCamera);
//                imagem = imagem.transformed(rotacao);

//                qDebug() << "Rodou";
//            }
//        }

//        if(imagem.width() != p_tamanhoImagem.width())
//        {
//            imagem = imagem.scaled(p_tamanhoImagem);
//        }

//        imagem = imagem.copy(p_rectRecorte);
//    }

//    return imagem;
//}

//void processaImagem::removeImagemSalva()
//{
//    QFile::remove(p_caminhoSalvar);
//}

