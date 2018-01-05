#include <Board.h>
#include <ToastBot.h>

#include "Counter.hpp"
#include "PhotoelectricSwitchSensor.hpp"
#include "PropertiesPage.hpp"
#include "ShopSensor.hpp"
#include "Webserver.hpp"

// *****************************************************************************
//                                  Arduino
// *****************************************************************************

int DEFAULT_PING_TIME = 5000;  // 5 seconds
int DEFAULT_UPDATE_TIME = 5000;  // 5 seconds
String DEFAULT_SERVER_URL = "";
int DEFAULT_SENSOR_PIN = 1;

WebServer webServer(80);

void setup()
{
   ToastBot::setup(new Esp8266Board());
   
   // Extract properties.
   Properties& properties = ToastBot::getProperties();
   int pingTime = properties.isSet("pingTime") ? properties.getInt("pingTime") : DEFAULT_PING_TIME;
   int updateTime = properties.isSet("updateTime") ? properties.getInt("updateTime") : DEFAULT_UPDATE_TIME;
   String serverUrl = properties.isSet("serverUrl") ? properties.getString("serverUrl") : DEFAULT_SERVER_URL;
   int sensorPin = properties.isSet("sensorPin") ? properties.getInt("sensorPin") : DEFAULT_SENSOR_PIN;
   
   Adapter* adapter = new HttpClientAdapter("httpAdapter", new JsonProtocol());
   ToastBot::addComponent(adapter);
   
   ShopSensor* shopSensor = new ShopSensor("httpAdapter", serverUrl, pingTime);
   ToastBot::addComponent(shopSensor, true);

   Logger::logDebug("Setting up sensor on pin %d", sensorPin);
   PhotoelectricSwitchSensor* sensor = new PhotoelectricSwitchSensor(sensorPin);
   ToastBot::addComponent(sensor);

   Counter* counter = new Counter("httpAdapter", serverUrl, updateTime, sensor);
   ToastBot::addComponent(counter);
   counter->start();

   webServer.setup();
   webServer.addPage(new PropertiesPage());
}

void loop()
{
   ToastBot::loop();

   webServer.loop();
}
