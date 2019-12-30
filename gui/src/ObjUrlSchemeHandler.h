#ifndef OBJURLSCHEMEHANDLER_H
#define OBJURLSCHEMEHANDLER_H

#include <QObject>
#include <QUrl>
#include <QWebEngineUrlRequestJob>
#include <QWebEngineUrlSchemeHandler>

class ObjUrlSchemeHandler : public QWebEngineUrlSchemeHandler
{
    Q_OBJECT

public:
    explicit ObjUrlSchemeHandler(QObject* parent = 0);
    void requestStarted(QWebEngineUrlRequestJob* request);

public slots:

private:
};

#endif // OBJURLSCHEMEHANDLER_H
