#ifndef QSPEXECWEBENGINEURLSCHEMEHANDLER_H
#define QSPEXECWEBENGINEURLSCHEMEHANDLER_H

#include <QObject>
#include <QUrl>
#include <QWebEngineUrlRequestJob>
#include <QWebEngineUrlSchemeHandler>

class ExecUrlSchemeHandler : public QWebEngineUrlSchemeHandler
{
    Q_OBJECT

public:
    explicit ExecUrlSchemeHandler(QObject* parent = 0);
    void requestStarted(QWebEngineUrlRequestJob* request);

public slots:

private:
};

#endif // QSPEXECWEBENGINEURLSCHEMEHANDLER_H
