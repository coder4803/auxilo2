
#include "messagewidgetpool.h"
#include "signalmessagewidget.h"

namespace MessageTool
{

MessageWidget *MessageWidgetFactory::operator()(const QString &type)
{
    if (type == "SignalMessage"){
        return new SignalMessageWidget();
    }
    // Other message types...
    else {
        // Unknown type.
        return 0;
    }
}


int MessageWidgetSelector::operator()(const QString& type)
{
    if (type == "SignalMessage"){
        return 0;
    }
    // Other message types...
    else{
        // Unknown type
        return -1;
    }
}


int MessageWidgetSelector::operator()(MessageWidget* obj)
{
    if (typeid(*obj) == typeid(SignalMessageWidget) ){
        return 0;
    }
    // Other message types...
    else{
        // Unknown type
        return -1;
    }
}



}
