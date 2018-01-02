#pragma once

#include "Component.hpp"
#include "Sensor.hpp"
#include "SensorListener.hpp"
#include "Timer.hpp"
#include "TimerListener.hpp"

class Counter : public Component, SensorListener, TimerListener
{

public:

   Counter();

   virtual ~Counter();

   void update();

   void setAdapter(
      const String& adapterId);

   void setSensor(
      ShopSensorNamespace::Sensor* sensor);

   void setUpdateTime(
      const int& updateTime);

   void start();

   void stop();

   // **************************************************************************
   //                        SensorListener implementation

   virtual void onTiggered();

   // **************************************************************************
   //                        TimerListener implementation

   virtual void timeout(
      Timer* timer);

   static const int DEFAULT_UPDATE_TIME;
   
private:

   bool sendCount();

   String adapterId;
   
   ShopSensorNamespace::Sensor* sensor;

   Timer* updateTimer;

   bool enabled;

   int updateTime;

   int count;
};
