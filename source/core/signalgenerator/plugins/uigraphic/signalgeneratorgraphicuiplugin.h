#ifndef SIGNALGENERATORGRAPHICUIPLUGIN_H
#define SIGNALGENERATORGRAPHICUIPLUGIN_H

#include "uiplugininterface.h"

class SignalGeneratorGraphicUiPlugin :
        public QObject,
        public SignalGenerator::UiPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID SIGNALGENERATOR_UIPLUGIN_IID)
    Q_INTERFACES(SignalGenerator::UiPluginInterface)

public:

    SignalGeneratorGraphicUiPlugin(QObject* parent = nullptr);

    virtual ~SignalGeneratorGraphicUiPlugin();

    // Implements SignalGenerator::UiPluginInterface
    virtual QCoreApplication* createApp(int &argc, char *argv[]);
    virtual SignalGenerator::ViewInterface* createView();
};

#endif // SIGNALGENERATORGRAPHICUIPLUGIN_H
