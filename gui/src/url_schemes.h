#ifndef URL_SCHEMES_H
#define URL_SCHEMES_H

#include "QspUrlSchemeHandler.h"
#include "ExecUrlSchemeHandler.h"
#include "ObjUrlSchemeHandler.h"

class RequestHandlers
{
public:
    RequestHandlers();
    ~RequestHandlers();
    static void register_url_schemes();
    void installUrlScheme();
    void SetGamePath(const QString& path);
private:
    QspUrlSchemeHandler* qspUriSchemeHandler;
    ExecUrlSchemeHandler* qspExecUriSchemeHandler;
    ObjUrlSchemeHandler* objSchemeHandler;
};

#endif // URL_SCHEMES_H
