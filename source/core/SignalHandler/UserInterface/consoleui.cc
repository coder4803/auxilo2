
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
#include <QCoreApplication>
#include <cstdio>
#include <csignal>


namespace
{

void systemSignalHandler(int signum)
{
    printf("Quited! (signal %i raised)\n", signum);
    QCoreApplication::quit();
}

}


ConsoleUI::ConsoleUI(bool verbose) : 
    SignalHandler::ViewInterface(), verbose_(verbose), mx_()
{
    signal(SIGINT, systemSignalHandler);
    signal(SIGTERM, systemSignalHandler);
}


ConsoleUI::~ConsoleUI()
{    
}


void ConsoleUI::critical(const QString& msg)
{
    std::lock_guard<std::mutex> lock(mx_);
    printf("%s\n", msg.toLatin1().data());
}

void ConsoleUI::debug(const QString& msg)
{
    if (verbose_){
        std::lock_guard<std::mutex> lock(mx_);
        printf("%s\n", msg.toLatin1().data());
    }
}

void ConsoleUI::warning(const QString& msg)
{
    if (verbose_){
        std::lock_guard<std::mutex> lock(mx_);
        printf("%s\n", msg.toLatin1().data());
    }
}


void ConsoleUI::fatal(const QString& msg)
{
    std::unique_lock<std::mutex> lock(mx_);
    printf("%s\n", msg.toLatin1().data());
    lock.unlock();
    abort();
}
