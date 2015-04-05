
#include <QCoreApplication>
#include "consoleui.hh"
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    ConsoleUI ui(&std::cin, &std::cout);
    a.exit( ui.exec() );
}
