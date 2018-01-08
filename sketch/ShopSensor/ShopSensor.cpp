#include "Logger.hpp"
#include "ShopSensor.hpp"
#include "ToastBot.hpp"
#include "WifiBoard.hpp"

ShopSensor::ShopSensor(
  const String& adapterId,
  const String& serverUrl,
  const int& pingTime) :
     Component("shopSensor"),
     adapterId(adapterId),
     serverUrl(serverUrl),
     pingTime(pingTime)
{
}

ShopSensor::~ShopSensor()
{
  Timer::freeTimer(pingTimer);
}

void ShopSensor::setup()
{
  pingTimer = Timer::newTimer("pingTimer", pingTime, Timer::PERIODIC, this);
  pingTimer->start();
}

void ShopSensor::handleMessage(
      MessagePtr message)
{
   Logger::logDebug("ShopSensor::handleMessage: %s", message->getMessageId().c_str());

   //  ping
   if (message->getMessageId() == "ping")
   {
      Message* reply = Messaging::newMessage();
      
      if (reply)
      {
         reply->setMessageId("pong");
         reply->setSource(getId());
         reply->setDestination(message->getSource());
         reply->set("deviceId", ToastBot::getId());

         if (WifiBoard::getBoard())
         {
           unsigned char mac[6] = {0, 0, 0, 0, 0, 0};
           WifiBoard::getBoard()->getMacAddress(mac);
           char macAddress[18];
           sprintf(macAddress, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);

           reply->set("macAddress", macAddress);
           reply->set("ipAddress", WifiBoard::getBoard()->getIpAddress());
         }

         Messaging::send(reply);

         message->setFree();
      }
   }
   else if (message->getMessageId() == "reset")
   {
      Board::getBoard()->reset();

      message->setFree();
   }
   else if (message->getMessageId() == "properties")
   {
      Properties& properties = ToastBot::getProperties();
    
      Logger::logDebug("ShopSensor::handleMessage: Properties:");
      properties.log();
   }
   else if (message->getMessageId() == "property")
   {
      String propertyName = message->getString("name");
      String propertyValue = message->getString("value");

      Properties& properties = ToastBot::getProperties();

      if (propertyName == "")
      {
         Logger::logDebug("ShopSensor::handleMessage: Properties:");
         properties.log();
      }
      else
      {
         if (propertyValue != "")
         {
            properties.set(propertyName, propertyValue);
            Logger::logDebug("ShopSensor::handleMessage: Updated properties: %s = %s", propertyName.c_str(), propertyValue.c_str());
         }
         else
         {
            properties.erase(propertyName);
            Logger::logDebug("ShopSensor::handleMessage: Removed property: %s", propertyName.c_str());
         }

         properties.save();
      }

      message->setFree();
   }
}

// **************************************************************************
//                        TimerListener implementation

void memoryTest()
{
   int bytes = 5;
   
   void* memoryPtr;
   
   while (memoryPtr = malloc(bytes))
   {
      free (memoryPtr);
      bytes += 5;
   }

   Logger::logDebug("Free heap: %d", ESP.getFreeHeap());
   Logger::logDebug("Largest chunk: %d bytes", bytes);
}

void ShopSensor::timeout(
   Timer* timer)
{
   Message* message = Messaging::newMessage();
   if (message)
   {
      message->setMessageId("sensor.php");
      message->setSource(ToastBot::getId());
      message->setDestination(adapterId);
      message->set("url", serverUrl);
      message->set("action", "ping");
      message->set("sensorId", ToastBot::getId());
       
      Messaging::send(message);
   }

   // Log free memory.
   memoryTest();
}

