#include "url_schemes.h"

#include <QWebEngineUrlScheme>
#include <QWebEngineProfile>

static const QByteArray QspWebEngineUrlSchemeName = QByteArrayLiteral("qsp");
static const QByteArray ObjUrlSchemeName = QByteArrayLiteral("obj");
static const QByteArray QspExecWebEngineUrlSchemeName = QByteArrayLiteral("exec");

RequestHandlers::RequestHandlers()
{
    qspUriSchemeHandler = new QspUrlSchemeHandler();
    qspExecUriSchemeHandler = new ExecUrlSchemeHandler();
    objSchemeHandler = new ObjUrlSchemeHandler();
}

RequestHandlers::~RequestHandlers()
{
    QWebEngineProfile::defaultProfile()->removeUrlSchemeHandler(qspUriSchemeHandler);
    delete qspUriSchemeHandler;

    QWebEngineProfile::defaultProfile()->removeUrlSchemeHandler(qspExecUriSchemeHandler);
    delete qspExecUriSchemeHandler;

    QWebEngineProfile::defaultProfile()->removeUrlSchemeHandler(objSchemeHandler);
    delete objSchemeHandler;
}

void RequestHandlers::installUrlScheme()
{
    QWebEngineProfile::defaultProfile()->installUrlSchemeHandler(QspWebEngineUrlSchemeName, qspUriSchemeHandler);
    QWebEngineProfile::defaultProfile()->installUrlSchemeHandler(QspExecWebEngineUrlSchemeName, qspExecUriSchemeHandler);
    QWebEngineProfile::defaultProfile()->installUrlSchemeHandler(ObjUrlSchemeName, objSchemeHandler);
}

void RequestHandlers::SetGamePath(const QString& path)
{
    qspUriSchemeHandler->SetGamePath(path);
}


void RequestHandlers::register_url_schemes()
{
    QWebEngineUrlScheme QspWebEngineUrlScheme(QspWebEngineUrlSchemeName);
    QspWebEngineUrlScheme.setSyntax(QWebEngineUrlScheme::Syntax::Path);
    QspWebEngineUrlScheme.setFlags(QWebEngineUrlScheme::SecureScheme |
        QWebEngineUrlScheme::LocalAccessAllowed |
        QWebEngineUrlScheme::ServiceWorkersAllowed |
        QWebEngineUrlScheme::ViewSourceAllowed |
        QWebEngineUrlScheme::ContentSecurityPolicyIgnored);

    QWebEngineUrlScheme::registerScheme(QspWebEngineUrlScheme);

    QWebEngineUrlScheme QspExecWebEngineUrlScheme(QspExecWebEngineUrlSchemeName);
    QspExecWebEngineUrlScheme.setSyntax(QWebEngineUrlScheme::Syntax::Path);

    QspExecWebEngineUrlScheme.setFlags(QWebEngineUrlScheme::SecureScheme |
        QWebEngineUrlScheme::LocalAccessAllowed |
        QWebEngineUrlScheme::ServiceWorkersAllowed |
        QWebEngineUrlScheme::ViewSourceAllowed |
        QWebEngineUrlScheme::ContentSecurityPolicyIgnored);

    QWebEngineUrlScheme::registerScheme(QspExecWebEngineUrlScheme);

    QWebEngineUrlScheme ObjWebEngineUrlScheme(ObjUrlSchemeName);
    ObjWebEngineUrlScheme.setSyntax(QWebEngineUrlScheme::Syntax::Path);

    ObjWebEngineUrlScheme.setFlags(QWebEngineUrlScheme::SecureScheme |
        QWebEngineUrlScheme::LocalAccessAllowed |
        QWebEngineUrlScheme::ServiceWorkersAllowed |
        QWebEngineUrlScheme::ViewSourceAllowed |
        QWebEngineUrlScheme::ContentSecurityPolicyIgnored);

    QWebEngineUrlScheme::registerScheme(ObjWebEngineUrlScheme);
}
