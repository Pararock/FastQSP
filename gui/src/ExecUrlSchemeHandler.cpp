#include "ExecUrlSchemeHandler.h"

#include <QString>
#include <QBuffer>
#include <QDir>
#include <QUrl>
#include <QMessageBox>
#include <QDebug>

#include <qsp/default/qsp_default.h>

ExecUrlSchemeHandler::ExecUrlSchemeHandler(QObject* parent) : QWebEngineUrlSchemeHandler(parent)
{

}

void ExecUrlSchemeHandler::requestStarted(QWebEngineUrlRequestJob* request)
{
    const QString path = request->requestUrl().path();
    qDebug() << "Request URL: " << request->requestUrl().toString();

    
    if (!QSPExecString(path.toStdWString().c_str(), true))
    {
        QString errorMessage;
        QSP_CHAR* loc;
        int code, actIndex, line;
        QSPGetLastErrorData(&code, &loc, &actIndex, &line);
        QString desc = QString::fromWCharArray(QSPGetErrorDesc(code));
        if (loc)
            errorMessage = QString("Location: %1\nArea: %2\nLine: %3\nCode: %4\nDesc: %5")
            .arg(QString::fromWCharArray(loc))
            .arg(actIndex < 0 ? QString("on visit") : QString("on action"))
            .arg(line)
            .arg(code)
            .arg(desc);
        else
            errorMessage = QString("Code: %1\nDesc: %2")
            .arg(code)
            .arg(desc);
        QMessageBox dialog(QMessageBox::Critical, tr("Error"), errorMessage, QMessageBox::Ok);
        dialog.exec();
        //QSPCallBacks::RefreshInt(QSP_FALSE);
    }

    request->redirect(QUrl("qsp://qspgame.local/"));
}
