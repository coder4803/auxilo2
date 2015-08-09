
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


ConsoleUI::ConsoleUI(bool verbose) : 
    SignalHandler::ViewInterface(), verbose_(verbose), mx_()
{
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
