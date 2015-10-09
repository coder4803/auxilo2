#include "signalgeneratorgraphicuiplugin.h"
#include <QApplication>
#include "mainwindow.h"


SignalGeneratorGraphicUiPlugin::
SignalGeneratorGraphicUiPlugin(QObject *parent):
    QObject(parent),
    SignalGenerator::UiPluginInterface()
{   
}


SignalGeneratorGraphicUiPlugin::~SignalGeneratorGraphicUiPlugin()
{
}


QCoreApplication*
SignalGeneratorGraphicUiPlugin::createApp(int &argc, char *argv[])
{
    return new QApplication(argc, argv);
}


SignalGenerator::ViewInterface *SignalGeneratorGraphicUiPlugin::createView()
{
    return new MainWindow();
}

