#pragma once

#include "Component.hpp"
#include "Sensor.hpp"
#include "SensorListener.hpp"

class PhotoelectricSwitchSensor : public Component, public ShopSensorNamespace::Sensor
{

public:

   PhotoelectricSwitchSensor(
      const int& readPin);

   virtual ~PhotoelectricSwitchSensor();

   virtual void loop();

   // **************************************************************************
   //                                Sensor interface

   virtual void setEnabled(
     const bool& isEnabled);

   virtual bool isEnabled();

   virtual void setListener(
      SensorListener* listener);

   virtual void update();

private:

   bool isSensorEnabled;

   int readPin;

   SensorListener* listener;

   bool isTriggered;

};
