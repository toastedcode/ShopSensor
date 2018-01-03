#include <Board.h>
#include <ToastBot.h>

#include "ConfigPage.hpp"
#include "Counter.hpp"
#include "PhotoelectricSwitchSensor.hpp"
#include "ShopSensor.hpp"

// *****************************************************************************
//                                  Arduino
// *****************************************************************************

int DEFAULT_SENSOR_PIN = 1;

void setup()
{
   ToastBot::setup(new Esp8266Board());

   Logger::setLogLevel(DEBUG_FINEST);

   ShopSensor* shopSensor = new ShopSensor();
   shopSensor->setAdapter("httpAdapter");
   ToastBot::addComponent(shopSensor, true);

   /*
   WebServerAdapter* webServerAdapter = new WebServerAdapter("web", 80);
   ToastBot::addComponent(webServerAdapter);
   webServerAdapter->addPage(new ConfigPage());
   */

   Adapter* adapter = new HttpClientAdapter("httpAdapter", new JsonProtocol());
   ToastBot::addComponent(adapter);

   Properties& properties = ToastBot::getProperties();

   int sensorPin = DEFAULT_SENSOR_PIN;
   if (properties.isSet("sensorPin"))
   {
      sensorPin = properties.getInt("sensorPin");
   }
   Logger::logDebug("Setting up sensor on pin %d", sensorPin);
   
   PhotoelectricSwitchSensor* sensor = new PhotoelectricSwitchSensor(sensorPin);
   ToastBot::addComponent(sensor);
   
   Counter* counter = new Counter();
   counter->setAdapter("httpAdapter");
   counter->setSensor(sensor);
   ToastBot::addComponent(counter);

   counter->start();
}

void loop()
{
   ToastBot::loop();
}
