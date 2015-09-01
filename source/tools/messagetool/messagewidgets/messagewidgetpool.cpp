
#include "messagewidgetpool.h"
#include "signalmessagewidget.h"
#include "confresponsemessagewidget.h"

namespace MessageTool
{

MessageWidget *MessageWidgetFactory::operator()(const QString &type)
{
    if (type == "SignalMessage"){
        return new SignalMessageWidget();
    }
    if (type == "ConfResponseMessage"){
        return new ConfResponseMessageWidget();
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
    else if (type == "ConfResponseMessage"){
        return 1;
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
    else if (typeid(*obj) == typeid(ConfResponseMessageWidget) ){
        return 1;
    }
    // Other message types...
    else{
        // Unknown type
        return -1;
    }
}



}
