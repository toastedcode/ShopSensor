#pragma once

#include "Component.hpp"
#include "Messaging.hpp"
#include "Timer.hpp"

class ShopSensor : public Component, TimerListener
{
  
public:
  
   // Constructor.
   ShopSensor(
      const String& adapterId,
      const String& serverUrl,
      const int& pingTime);

   // Destructor.
   virtual ~ShopSensor();

   virtual void setup();

   // This operation handles a message directed to this sensor.
   virtual void handleMessage(
      // The message to handle.
      MessagePtr message);

   // **************************************************************************
   //                        TimerListener implementation

   virtual void timeout(
      Timer* timer);

private:

   String adapterId;

   String serverUrl;

   int pingTime;

   Timer* pingTimer;
};

