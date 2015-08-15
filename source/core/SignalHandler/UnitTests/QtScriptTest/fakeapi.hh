#ifndef FAKEAPI_HH
#define FAKEAPI_HH

#include "scriptapi.hh"
#include <map>
#include <QList>


class FakeAPI : public SignalHandler::ScriptAPI
{
public:
    
    FakeAPI();
    
    virtual ~FakeAPI();
    
    virtual QDateTime dateTimeNow() const;
    
    virtual Utils::StateResponseMessage::State 
    getStateOf(const QString& stateName);
    
    virtual ScriptAPI::StateMap getStates(const QStringList& states);
    
    virtual int setState(const QString& stateName, const QVariant& value);
    
    virtual int sendSignal(const QString& signalName, const QStringList& args,
                           const QString& target = QString());
    
    virtual QString getSender() const;
    
    // Verification functions:
    bool verifyCalledOnly(QList<int> indecies);
    void clear();
    
private:
    
    mutable std::vector<bool> called_fns_;
    ScriptAPI::StateMap virtual_states_;
    
    static const int dateTimeNowCalled_ = 0;
    static const int getStateOfCalled_ = 1;
    static const int getStatesCalled_ = 2;
    static const int setStateCalled_ = 3;
    static const int sendSignalCalled_ = 4;
};

#endif // FAKEAPI_HH
