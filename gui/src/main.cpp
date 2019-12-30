#include "fastqspwindow.h"
#include <QApplication>
#include <QtDebug>
#include <omp.h>

#include <QtWebEngine>
#include "url_schemes.h"

#define STRINGIZE_(x) #x
#define STRINGIZE(x) STRINGIZE_(x)

FastQSPWindow* qspWin;

void myMessageOutput(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    if (type == QtDebugMsg)
    {
        QFile outFile("log");
        outFile.open(QIODevice::WriteOnly | QIODevice::Append);
        QTextStream ts(&outFile);
        ts << msg << endl;
    }
}
void myMessageHandler(QtMsgType type, const char* msg)
{
    QString txt;
    if (type == QtDebugMsg)
    {
        txt = QString(msg);
    }
    QFile outFile("log");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << endl;
}

int main(int argc, char* argv[]) {
    //      qInstallMessageHandler(myMessageOutput);
    QCoreApplication::setApplicationName("FastQSP");
    QCoreApplication::setApplicationVersion(STRINGIZE(GIT_VERSION));

    
    RequestHandlers::register_url_schemes();
    QtWebEngine::initialize();

    QApplication a(argc, argv);

    qspWin = new FastQSPWindow();
    qspWin->resize(975, 630);

    qspWin->show();

    if (argc > 1)
    {
        QFile file(argv[1]);
        if (file.exists()) {
            qspWin->openFile(file.fileName());
        }        
    }

    return a.exec();
}
