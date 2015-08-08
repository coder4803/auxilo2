
/* consoleui.cc
 * 
 * This is the implementation file for the ConsoleUI class defined in
 * consoleui.hh.
 * 
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Created: 06-April-2015
 * Last modified: 06-April-2015
 */

#include "consoleui.hh"
#include <QDebug>
#include <functional>
#include <cstdio>

namespace
{

bool verbose_;
std::mutex outputMx_;

void messageHandler(QtMsgType type, 
                    const QMessageLogContext& context, 
                    const QString& msg)
{
    Q_UNUSED (context);
    
    switch (type) {
    case QtWarningMsg:
    case QtDebugMsg:
        if (verbose_){
            std::lock_guard<std::mutex> lock(outputMx_);
            printf("%s\n", msg.toLatin1().data());
        }
        break;
        
    case QtCriticalMsg:
    {
        std::lock_guard<std::mutex> lock(outputMx_);
        printf("%s\n", msg.toLatin1().data());
    }
        break;
        
    case QtFatalMsg:
    {
        std::lock_guard<std::mutex> lock(outputMx_);
        printf("%s\n", msg.toLatin1().data());
    }
        abort();
        
    default:
        break;
    }
}

} // Anonymous namespace ends.


ConsoleUI::ConsoleUI(bool verbose)
{
    verbose_ = verbose;
    qInstallMessageHandler(messageHandler);
}


ConsoleUI::~ConsoleUI()
{    
}


void ConsoleUI::critical(const QString& msg)
{
    qCritical() << msg;
}

void ConsoleUI::debug(const QString& msg)
{
    qDebug() << msg;
}

void ConsoleUI::warning(const QString& msg)
{
    qWarning() << msg;
}


void ConsoleUI::fatal(const QString& msg)
{
    qFatal(msg.toLatin1().data());
}
