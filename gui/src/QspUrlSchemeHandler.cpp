#include "QspUrlSchemeHandler.h"

#include <QBuffer>
#include <QUrl>
#include <QMimeType>
#include <QFile>
#include <QDebug>

QspUrlSchemeHandler::QspUrlSchemeHandler(QObject* parent) : QWebEngineUrlSchemeHandler(parent)
{

}

void QspUrlSchemeHandler::requestStarted(QWebEngineUrlRequestJob* request)
{
    const QUrl url = request->requestUrl();
    const QString requestPath = request->requestUrl().path();
    QBuffer* buffer = new QBuffer;
    connect(request, SIGNAL(destroyed()), buffer, SLOT(deleteLater()));

    buffer->open(QIODevice::WriteOnly);
    bool isFileRequest = true;
    if (url.host() == "qspgame.local") {
        if (requestPath == "/" || requestPath.isEmpty())
        {
            // This is the path that send back the rendered Main Description in html
            builder.clear();
            QString html = builder.getHTML();
            if (builder.thereIsAMessage) {
                qDebug() << "There's a message ";
            }
            buffer->write(html.toUtf8());
            buffer->close();
            request->reply("text/html", buffer);
            isFileRequest = false;
        }
        else
        {
            // Here we get all other qsp://qspgame.local requests
            QString cleanPath = QDir::cleanPath(requestPath.mid(1));

            int index = cleanPath.indexOf("/");
            if (index == -1) {
                // No sub folder, check if it's an action click send in the qsp://{int} format
                bool ok = false;
                int actionNumber = cleanPath.toInt(&ok);
                if (ok) {
                    QSPSetSelActionIndex(actionNumber - 1, true);
                    QSPExecuteSelActionCode(true);
                    buffer->close();
                    request->redirect(QUrl("qsp://qspgame.local/"));
                    isFileRequest = false;
                }
            }

            if (isFileRequest) {
                QString path = m_dir.absoluteFilePath(cleanPath);
                QMimeType type = db.mimeTypeForFile(path);
                QFile file(path);
                if (file.open(QIODevice::ReadOnly))
                {
                    buffer->write(file.readAll());
                    buffer->close();
                    file.close();
                    request->reply(type.name().toUtf8(), buffer);
                }
                else {
                    qDebug() << "Request URL: " << request->requestUrl().toString() << " not found";
                    request->fail(QWebEngineUrlRequestJob::Error::UrlNotFound);
                }
            }
        }
    }
    else {
        qDebug() << "Bad host: " << request->requestUrl().toString();
        request->fail(QWebEngineUrlRequestJob::Error::UrlInvalid);
    }
}

void QspUrlSchemeHandler::SetGamePath(const QString& path)
{
    m_dir = QDir(path);
}
