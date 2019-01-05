#include <App.hpp>
#include <fauxmoESP.h>
#include <OpenHabHandler.hpp>
#include <MqttHandler.hpp>
#include <AlexaHandler.hpp>
#include <Syslog.hpp>
#include "RelayHandler.hpp"

RelayHandler relayHandler;

RelayHandler::RelayHandler()
{
  delayAction = false;
  delayState = false;
  powerOn = false;
}

void RelayHandler::on()
{
  LOG0( "Switch power ON\n" );

  if( powerOn == false )
  {
    #if OBI_VERSION == 1
      digitalWrite( RELAY_TRIGGER_ON, 0 );
      delay(5);
      digitalWrite( RELAY_TRIGGER_ON, 1 );
    #endif

    #if OBI_VERSION == 2 || DEVELOPMENT_VERSION == 1
      digitalWrite( POWER_LED, 1 );
      digitalWrite( RELAY_PIN, 1 );
    #endif

    powerOn = true;
    openHabHandler.sendValue("ON");
    mqttHandler.sendValue("ON");
    alexaHandler.sendValue(true);
    syslog.logInfo( "power on" );
  }
}

void RelayHandler::off()
{
  LOG0( "Switch power OFF\n" );

  if( powerOn == true )
  {
    #if OBI_VERSION == 1
      digitalWrite( RELAY_TRIGGER_OFF, 0 );
      delay(5);
      digitalWrite( RELAY_TRIGGER_OFF, 1 );
    #endif

    #if OBI_VERSION == 2 || DEVELOPMENT_VERSION == 1
      digitalWrite( POWER_LED, 0 );
      digitalWrite( RELAY_PIN, 0 );
    #endif

    powerOn = false;
    openHabHandler.sendValue("OFF");
    mqttHandler.sendValue("OFF");
    alexaHandler.sendValue(false);
    syslog.logInfo( "power off" );
  }
}

void RelayHandler::toggle()
{
  if( powerOn )
  {
    delayedOff();
  }
  else
  {
    delayedOn();
  }
}

const bool RelayHandler::isPowerOn()
{
  return powerOn;
}

void RelayHandler::delayedOn()
{
  if( powerOn == false )
  {
    LOG0("delayedOn\n");
    delayAction = true;
    delayState = true;
  }
}

void RelayHandler::delayedOff()
{
  if( powerOn == true )
  {
    LOG0("delayedOff\n");
    delayAction = true;
    delayState = false;
  }
}

void RelayHandler::handle()
{
  if ( delayAction )
  {
    delayAction = false;

    if( delayState )
    {
      on();
    }
    else
    {
      off();
    }
  }
}
