#ifndef QSPWEBENGINEURLSCHEMEHANDLER_H
#define QSPWEBENGINEURLSCHEMEHANDLER_H

#include <QObject>
#include <QString>
#include <QDir>
#include <QWebEngineUrlRequestJob>
#include <QWebEngineUrlSchemeHandler>
#include <QMimeDatabase>
#include "qsp_htmlbuilder.h"

class QspUrlSchemeHandler : public QWebEngineUrlSchemeHandler
{
    Q_OBJECT

public:
    explicit QspUrlSchemeHandler(QObject* parent = 0);
    void requestStarted(QWebEngineUrlRequestJob* request);
    void SetGamePath(const QString& path);

private:
    QSP_HTMLBuilder builder;
    QDir m_dir;
    QMimeDatabase db;
};

#endif // QSPWEBENGINEURLSCHEMEHANDLER_H
