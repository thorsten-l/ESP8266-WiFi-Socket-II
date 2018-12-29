#include <App.hpp>
#include <OpenHabHandler.hpp>
#include <MqttHandler.hpp>
#include <AlexaHandler.hpp>
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
    digitalWrite( POWER_LED, 1 );
    digitalWrite( RELAY_PIN, 1 );
    powerOn = true;
    openHabHandler.sendValue("ON");
    mqttHandler.sendValue("ON");
    alexaHandler.sendValue(true);
  }
}

void RelayHandler::off()
{
  LOG0( "Switch power OFF\n" );

  if( powerOn == true )
  {
    digitalWrite( POWER_LED, 0 );
    digitalWrite( RELAY_PIN, 0 );
    powerOn = false;
    openHabHandler.sendValue("OFF");
    mqttHandler.sendValue("OFF");
    alexaHandler.sendValue(false);
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
