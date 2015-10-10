/*
 * userinterface.cpp
 * This is the implementation file for the ConsoleUI class defined in
 * consoleui.h.
 *
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Date: 10-Oct-2015
 */

#include "consoleui.h"
#include <cstdio>
#include <csignal>
#include <QCoreApplication>


namespace
{
    // Handle environment signals.
    void handleSignals(int signum)
    {
        printf("Quited! (signal %i was risen)\n", signum);
        QCoreApplication::quit();
    }
}


namespace SignalGenerator
{

ConsoleUI::ConsoleUI() :
    ViewInterface(), verbose_(false), model_(nullptr)
{
    signal(SIGINT, handleSignals);
    signal(SIGABRT, handleSignals);
    signal(SIGTERM, handleSignals);
}


ConsoleUI::~ConsoleUI()
{
}


void ConsoleUI::debug(const QString &msg)
{
    if (verbose_){
        printf("%s\n", msg.toStdString().data());
    }
}


void ConsoleUI::warning(const QString &msg)
{
    if (verbose_){
        printf("%s\n", msg.toStdString().data());
    }
}


void ConsoleUI::critical(const QString &msg)
{
    printf("%s\n", msg.toStdString().data());
}


void ConsoleUI::fatal(const QString &msg)
{
    printf("%s\n", msg.toStdString().data());
    abort();
}


void ConsoleUI::setVerbose(bool value)
{
    verbose_ = value;
}


void ConsoleUI::setModel(ModelInterface *model)
{
    model_ = model;
}


} // namespace SignalGenerator
