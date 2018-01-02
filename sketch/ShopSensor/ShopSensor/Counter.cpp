#include "Counter.hpp"
#include "Logger.hpp"
#include "Messaging.h"
#include "ToastBot.h"

// *****************************************************************************
//                                    Public
// *****************************************************************************

const int Counter::DEFAULT_UPDATE_TIME = 1000;  // 1 second

Counter::Counter() :
   Component("partCounter"),
   adapterId(""),
   sensor(0),
   enabled(false),
   updateTime(DEFAULT_UPDATE_TIME),
   count(0)
{
   Properties properties = ToastBot::getProperties();

   if (properties.isSet("updateTime"))
   {
      updateTime = properties.getInt("updateTime");
   }
   
   updateTimer = Timer::newTimer("counterTimer", updateTime, Timer::PERIODIC, this);
   updateTimer->start();
}

Counter::~Counter()
{
  Timer::freeTimer(updateTimer);
}

void Counter::update()
{
}

void Counter::setAdapter(
   const String& adapterId)
{
   this->adapterId = adapterId;
}

void Counter::setSensor(
   ShopSensorNamespace::Sensor* sensor)
{
   this->sensor = sensor;
   sensor->setListener(this);
}

void Counter::setUpdateTime(
  const int& updateTime)
{
   this->updateTime = updateTime;
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
      message->set("url", "www.roboxes.com/pptp");
      message->set("count", count);

      sent = Messaging::send(message);
   }

   return (sent);
}
