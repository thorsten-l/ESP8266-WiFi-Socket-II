#include <App.hpp>
#ifdef HAVE_HLW8012
#ifndef __HLW8012_HANDLER_H__
#define __HLW8012_HANDLER_H__

#include <Arduino.h>
#include <HLW8012.h>

class Hlw8012Handler
{
private:
  unsigned long lastReadTimestamp = 0;
  double voltage;
  double current;
  int activePower;

public:
  void setup();
  void handle(unsigned long timestamp);
  double getVoltage();
  double getCurrent();
  double getPower();
  double getActivePower();
};

extern Hlw8012Handler hlw8012Handler;
extern HLW8012 hlw8012;

#endif
#endif
