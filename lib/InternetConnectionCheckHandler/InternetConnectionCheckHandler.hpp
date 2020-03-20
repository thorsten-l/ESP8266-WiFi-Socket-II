#ifndef __INTERNET_CONNECTION_CHECK_HANDLER_HPP__
#define __INTERNET_CONNECTION_CHECK_HANDLER_HPP__

#include <Arduino.h>

class InternetConnectionCheckHandler
{
  private:
    bool initialized;
    time_t lastCheckedTimestamp;
    void setup();

  public:
    InternetConnectionCheckHandler();
    void handle( time_t now );
};

extern InternetConnectionCheckHandler internetConnectionCheckHandler;

#endif
