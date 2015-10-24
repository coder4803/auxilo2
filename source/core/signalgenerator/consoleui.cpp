/*
 * userinterface.cpp
 * This is the implementation file for the ConsoleUI class defined in
 * consoleui.h.
 *
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Date: 10-Oct-2015
 */

#include "consoleui.h"
#include <iostream>
#include <string>
#include <cstdio>
#include <csignal>
#include <QCoreApplication>
#include <QSqlQuery>


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
    ViewInterface(), verbose_(false), model_(nullptr),
    output_mx_(), input_thread_()
{
    input_thread_ = std::move(std::thread(&ConsoleUI::readInput, this));
    input_thread_.detach();

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
        std::lock_guard<std::mutex> lock(output_mx_);
        std::cout << "Debug: " << msg.toStdString() << std::endl;
        std::cout << ">> ";
    }
}


void ConsoleUI::warning(const QString &msg)
{
    if (verbose_){
        std::lock_guard<std::mutex> lock(output_mx_);
        std::cout << "Warning: " << msg.toStdString() << std::endl;
        std::cout << ">> ";
    }
}


void ConsoleUI::critical(const QString &msg)
{
    std::lock_guard<std::mutex> lock(output_mx_);
    std::cout << msg.toStdString() << std::endl;
    std::cout << ">> ";
}


void ConsoleUI::fatal(const QString &msg)
{
    output_mx_.lock();
    std::cout << "Fatal: " << msg.toStdString() << std::endl;
    output_mx_.unlock();
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


void ConsoleUI::readInput()
{
    output_mx_.lock();
    std::cout << "Type 'help' for help." << std::endl;
    output_mx_.unlock();

    while (true)
    {
        std::string input;
        std::getline(std::cin, input);

        if (input == "quit"){
            QCoreApplication::exit();
            return;
        }
        else if (input == "list"){
            this->printTaskList();
        }
        else if (input == "events"){
            this->printEventsTable();
        }
        else if (input == "help"){
            this->printHelp();
        }
        else{
            output_mx_.lock();
            std::cout << "Unknown command: " << input << std::endl;
            output_mx_.unlock();
        }

        std::cout << ">> ";
    }
}


void ConsoleUI::printTaskList()
{
    QSqlQuery list = model_->getTaskList()->query();
    list.exec();
    std::lock_guard<std::mutex> lock(output_mx_);

    std::cout << "Task list:" << std::endl;
    std::cout << "timestamp \t\t" << "signal \t" << "interval \t" << "repeat\n";
    for (int i=0; i<80; ++i) {std::cout << '-';}
    std::cout << std::endl;

    while (list.next())
    {
        std::cout << list.value("timestamp").toString().toStdString() << "\t";
        std::cout << list.value("signal").toString().toStdString() << "\t";
        std::cout << list.value("interval").toString().toStdString() << "\t";
        std::cout << list.value("repeat").toString().toStdString() << "\n";
    }
}


void ConsoleUI::printEventsTable()
{
    QSqlQuery table = model_->getEventTable()->query();
    table.exec();
    std::lock_guard<std::mutex> lock(output_mx_);

    std::cout << "Events:" << std::endl;
    std::cout << "id\t" << "signal \t" << "timestamp \t\t"
              << "interval \t" << "repeat\t" << "static" << std::endl;
    for (int i=0; i<80; ++i) {std::cout << '-';}
    std::cout << std::endl;

    while (table.next())
    {
        std::cout << table.value("id").toInt() << "\t";
        std::cout << table.value("signal").toString().toStdString() << "\t";
        std::cout << table.value("timestamp").toString().toStdString() << "\t";
        std::cout << table.value("interval").toString().toStdString() << "\t";
        std::cout << table.value("repeat").toString().toStdString() << "\t";
        std::cout << table.value("static").toInt() << "\n";
    }
}


void ConsoleUI::printHelp()
{
    std::lock_guard<std::mutex> lock(output_mx_);
    std::cout << "Commands:"    << std::endl;
    std::cout << "list:\t"       << "Print task list."   << std::endl;
    std::cout << "events:\t"     << "Print event table"  << std::endl;
    std::cout << "quit:\t"       << "Quit program"       << std::endl;
}


} // namespace SignalGenerator
