#ifndef BUSINESSLOGIC_H
#define BUSINESSLOGIC_H

#include <QObject>
#include "confreader.h"
#include "eventmanager.h"


namespace SignalGenerator
{

/*!
 * \brief The BusinessLogic class
 * The central logic coomponent, that encapsulates all other
 * components into one.
 */
class BusinessLogic : public QObject
{
    Q_OBJECT

public:

    BusinessLogic();

    ~BusinessLogic();
    BusinessLogic(const BusinessLogic&) = delete;
    BusinessLogic& operator=(const BusinessLogic&) = delete;


private slots:

    void onConfigurationReceived();

private:

    ConfReader confReader_;
    EventManager* eventManager_;
};

}

#endif // BUSINESSLOGIC_H
