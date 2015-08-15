#include "fakeapi.hh"

FakeAPI::FakeAPI() : 
    SignalHandler::ScriptAPI(),
    called_fns_(5, false), virtual_states_()
{
}

FakeAPI::~FakeAPI()
{
}


QDateTime FakeAPI::dateTimeNow() const
{
    called_fns_.at(dateTimeNowCalled_) = true;
    return QDateTime::currentDateTime();
}

Utils::StateResponseMessage::State FakeAPI::getStateOf(const QString& stateName)
{
    called_fns_.at(getStateOfCalled_) = true;
    return virtual_states_.value(stateName);
}

SignalHandler::ScriptAPI::StateMap FakeAPI::getStates(const QStringList& states)
{
    called_fns_.at(getStatesCalled_) = true;
    ScriptAPI::StateMap result;
    
    foreach (QString s, states) {
        result.insert(s, virtual_states_.value(s) );
    }
    return result;
}

int FakeAPI::setState(const QString& stateName, const QVariant& value)
{
    called_fns_.at(setStateCalled_) = true;
    Utils::StateResponseMessage::State st(true, value, true);
    virtual_states_.insert(stateName, st);
    return 0;
}

int FakeAPI::sendSignal(const QString& signalName, const QStringList& args, 
                        const QString& target)
{
    Q_UNUSED(signalName); Q_UNUSED(args); Q_UNUSED(target);
    called_fns_.at(sendSignalCalled_) = true;
    return 0;
}

QString FakeAPI::getSender() const
{
    return QString();
}

bool FakeAPI::verifyCalledOnly(QList<int> indecies)
{
    for (int i=0; i<5; ++i){
        if (indecies.contains(i)){
            if (!called_fns_.at(i)){
                return false;
            }
        }
        else if (called_fns_.at(i)){
            return false;
        }
    }
    return true;
}

void FakeAPI::clear()
{
    for (unsigned i=0; i<called_fns_.size(); ++i){
        called_fns_.at(i) = false;
    }
    virtual_states_.clear();
}
