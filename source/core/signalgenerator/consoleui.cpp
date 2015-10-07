#include "consoleui.h"
#include <cstdio>
#include <csignal>
#include <QCoreApplication>


namespace
{
    void handleSignals(int signum)
    {
        printf("Quited! (signal %i was risen)\n", signum);
        QCoreApplication::quit();
    }
}


namespace SignalGenerator
{

ConsoleUI::ConsoleUI() :
    ViewInterface(), verbose_(false)
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


void ConsoleUI::setTableModel(QSqlTableModel *model)
{
    Q_UNUSED(model);
    this->warning("UI does not support showing database contents");
}

} // namespace SignalGenerator
