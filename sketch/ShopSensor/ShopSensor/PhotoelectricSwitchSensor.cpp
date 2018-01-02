#include "Arduino.h"
#include "Logger.hpp"
#include "PhotoelectricSwitchSensor.hpp"

static const int TRIGGER_THRESHOLD = 50;

PhotoelectricSwitchSensor::PhotoelectricSwitchSensor(
   const int& readPin) :
      Component("photoelectricSwitchSensor"),
      isSensorEnabled(false),
      readPin(readPin),
      listener(0),
      isTriggered(false)
{
   pinMode(readPin, INPUT);
}

PhotoelectricSwitchSensor::~PhotoelectricSwitchSensor()
{
}

void PhotoelectricSwitchSensor::loop()
{
   update();
}

inline void PhotoelectricSwitchSensor::setEnabled(
  const bool& isEnabled)
{
   isSensorEnabled = isEnabled;
}

inline bool PhotoelectricSwitchSensor::isEnabled()
{
   return (isSensorEnabled);
}

inline void PhotoelectricSwitchSensor::setListener(
   SensorListener* listener)
{
   this->listener = listener;
}

void PhotoelectricSwitchSensor::update()
{
   int sensorValue = digitalRead(readPin);

   bool wasTriggered = isTriggered;

   isTriggered = (sensorValue > TRIGGER_THRESHOLD);

   if (!wasTriggered && isTriggered)
   {
      Logger::logDebug("Triggered");

      if (listener != 0)
      {
         listener->onTiggered();
      }
   }
   else if (wasTriggered && !isTriggered)
   {
      Logger::logDebug("Untriggered");
   }
}
