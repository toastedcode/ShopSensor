#pragma once

#include "Component.hpp"
#include "Messaging.hpp"
#include "Timer.hpp"

class ShopSensor : public Component, TimerListener
{
  
public:
  
   // Constructor.
   ShopSensor();

   // Destructor.
   virtual ~ShopSensor();

   void setAdapter(
      const String& adapterId);

   virtual void setup();

   // This operation handles a message directed to this sensor.
   virtual void handleMessage(
      // The message to handle.
      MessagePtr message);

   // **************************************************************************
   //                        TimerListener implementation

   virtual void timeout(
      Timer* timer);

   static const int DEFAULT_PING_TIME = 1000;  // 1 second

private:

   String adapterId;

   Timer* pingTimer;

   int pingTime;
};

