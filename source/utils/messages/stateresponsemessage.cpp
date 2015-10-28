#include <QDataStream>

#include "stateresponsemessage.h"

namespace Utils {

StateResponseMessage::State::State(bool Available,
      QVariant Value,
      bool DevicesAreUpToDate) :
   available(Available),
   value(Value),
   devicesAreUpToDate(DevicesAreUpToDate)
{}

StateResponseMessage::State::State(const State& other) :
   available(other.available),
   value(other.value),
   devicesAreUpToDate(other.devicesAreUpToDate)
{}

StateResponseMessage::State
StateResponseMessage::State::operator=(const StateResponseMessage::State& other)
{
   if (&other != this) {
      available = other.available;
      value = other.value;
      devicesAreUpToDate = other.devicesAreUpToDate;
   }

   return *this;
}

StateResponseMessage::StateResponseMessage() :
   Message()
{
}

StateResponseMessage::StateResponseMessage(const QByteArray& payload) :
   Message()
{
   QDataStream stream(payload);

   quint32 numberOfStates;
   stream >> numberOfStates;

   for (quint32 i = 0; i < numberOfStates; ++i) {
      QString name;
      stream >> name;

      bool available;
      stream >> available;

      if (available) {
         QVariant value;
         stream >> value;

         bool devicesAreUpToDate;
         stream >> devicesAreUpToDate;
         m_states.insert(name, new State(available, value, devicesAreUpToDate));
      } else {
         m_states.insert(name, new State(false));
      }
   }
}

StateResponseMessage::StateResponseMessage(const StateResponseMessage &other)
{
   foreach (State* state, other.states()) {
      this->appendState(other.states().key(state), *state);
   }
}

StateResponseMessage
StateResponseMessage::operator=(const StateResponseMessage &other)
{
   if (&other != this) {
      foreach (State* state, other.states()) {
         this->appendState(other.states().key(state), *state);
      }
   }

   return *this;
}

StateResponseMessage::~StateResponseMessage()
{
   foreach (State* state, m_states) {
      delete state;
   }
   m_states.clear();
}

void StateResponseMessage::appendState(QString name, State state)
{
   m_states.insert(name, new State(state));
}

StateResponseMessage::State StateResponseMessage::state(QString name) const
{
   Q_ASSERT(m_states.contains(name));
   return *m_states.value(name);
}

QByteArray StateResponseMessage::data() const
{
   QByteArray message;
   QDataStream stream(&message, QIODevice::WriteOnly);

   stream << m_states.size();

   foreach (State* state, m_states) {
      stream << m_states.key(state);
      stream << state->available;

      if (state->available) {
         stream << state->value;
         stream << state->devicesAreUpToDate;
      }
   }

   return message;
}

QString StateResponseMessage::string() const
{
   QString message("StateResponseMessage:\n");

   foreach (State* state, m_states) {
      message += QString("- Name: %1").arg(m_states.key(state));

      if (state->available) {
         QString boolStr = state->devicesAreUpToDate ? "upToDate" : "notUpToDate";
         message += QString(", %1").arg(boolStr);

         message += QString(", Value: %1").arg(state->value.toByteArray().toHex().data());
      } else {
         message += QString(", not available");
      }

      message += QString("\n");
   }

   return message;
}

} // Utils
