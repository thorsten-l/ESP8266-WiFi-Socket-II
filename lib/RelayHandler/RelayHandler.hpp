#ifndef __RELAY_HANDLER_HPP__
#define __RELAY_HANDLER_HPP__

class RelayHandler
{
private:
  volatile bool powerOn;
  volatile bool delayState;
  volatile bool delayAction;
  void on();
  void off();

public:
  RelayHandler();

  const bool isPowerOn();
  const bool isDelayedPowerOn();
  void toggle();
  void delayedOn();
  void delayedOff();
  void handle();
};

extern RelayHandler relayHandler;

#endif
