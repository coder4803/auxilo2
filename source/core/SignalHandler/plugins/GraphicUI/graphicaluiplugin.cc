/* graphicaluiplugin.cc
 * 
 * This is the implementation file for the GraphicalUiPlugin plugin class
 * defined in graphicaluiplugin.hh.
 * 
 * Author: Perttu Paarlahti     perttu.paarlahti@gmail.com
 * Created: 12-Aug-2015
 */ 

#include "graphicaluiplugin.hh"
#include "graphicalui.hh"
#include <QApplication>
#include <QDebug>


GraphicalUiPlugin::GraphicalUiPlugin(QObject* parent) : QObject(parent)
{
}


GraphicalUiPlugin::~GraphicalUiPlugin()
{    
}


QCoreApplication*GraphicalUiPlugin::createApp(int& argc, char* argv[])
{
    Q_ASSERT (argc > 0);
    Q_ASSERT (argv != nullptr);
    Q_ASSERT (QCoreApplication::instance() == nullptr);
    
    return new QApplication(argc, argv);
}


SignalHandler::ViewInterface*GraphicalUiPlugin::createView(bool verbose)
{
    return new GraphicalUI(verbose);
}

