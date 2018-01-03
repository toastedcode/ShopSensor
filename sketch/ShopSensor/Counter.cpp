#include "Counter.hpp"
#include "Logger.hpp"
#include "Messaging.h"
#include "ToastBot.h"

// *****************************************************************************
//                                    Public
// *****************************************************************************

Counter::Counter(
   const String& adapterId,
   const String& serverUrl,
   const int& updateTime,
   ShopSensorNamespace::Sensor* sensor) :
      Component("partCounter"),
      adapterId(adapterId),
      serverUrl(serverUrl),
      updateTime(updateTime),
      sensor(sensor),
      enabled(false),
      count(0)
{
   sensor->setListener(this);
  
   updateTimer = Timer::newTimer("counterTimer", updateTime, Timer::PERIODIC, this);
   updateTimer->start();
}

Counter::~Counter()
{
  Timer::freeTimer(updateTimer);
}

void Counter::start()
{
   enabled = true;

   if (sensor)
   {
      sensor->setEnabled(true);
   }

   if (updateTimer)
   {
      updateTimer->start();
   }
}

void Counter::stop()
{
   enabled = false;

   if (sensor)
   {
      sensor->setEnabled(false);
   }

   if (updateTimer)
   {
      updateTimer->stop();
   }
}

// **************************************************************************
//                        SensorListener implementation

void Counter::onTiggered()
{
   Logger::logDebug("onTriggered");

   if (enabled)
   {
      count++;
   }
}

// **************************************************************************
//                        TimerListener implementation

void Counter::timeout(
   Timer* timer)
{
   //Logger::logDebug("onTimeout");

   if (enabled)
   {
      // If we have any counts to report ...
      if (count > 0)
      {
         if (sendCount() == true)
         {
            count = 0;
         }
      }
   }
   else
   {
      // Stop this rogue timer.
      updateTimer->stop();
   }
}

// *****************************************************************************
//                                    Private
// *****************************************************************************

bool Counter::sendCount()
{
   bool sent = false;
  
   Message* message = Messaging::newMessage();
   if (message)
   {
      message->setMessageId("count");
      message->setSource(ToastBot::getId());
      message->setDestination(adapterId);
      message->set("url", serverUrl);
      message->set("count", count);

      sent = Messaging::send(message);
   }

   return (sent);
}
