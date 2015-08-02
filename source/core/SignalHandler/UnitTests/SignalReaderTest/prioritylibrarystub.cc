#include "prioritylibrarystub.hh"
#include "unknownscript.hh"
#include <QDebug>

PriorityLibraryStub::PriorityLibraryStub() :
    ScriptPriorityLibrary(), data_()
{
}


PriorityLibraryStub::~PriorityLibraryStub()
{
}


unsigned PriorityLibraryStub::getPriorityOf(const QString& scriptID) const
{
    auto it = data_.find(scriptID);
    if (it == data_.end()){
        throw SignalHandler::UnknownScript(scriptID);
    }
    return it->second;
}


void PriorityLibraryStub::addScript(const QString& name, unsigned priority)
{
    data_[name] = priority;
}


void PriorityLibraryStub::clear()
{
    data_.clear();
}
