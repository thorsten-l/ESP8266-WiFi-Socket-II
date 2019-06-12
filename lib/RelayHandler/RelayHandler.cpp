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
    powerOn = true;

    #if defined(BOARD_TYPE_OBI_V1)
      app.showLeds();
      digitalWrite( RELAY_TRIGGER_ON, 0 );
      delay(5);
      digitalWrite( RELAY_TRIGGER_ON, 1 );
    #endif

    #if defined(BOARD_TYPE_OBI_V2) || defined(BOARD_TYPE_DEV1) || defined(BOARD_TYPE_BW_SHP6) || defined(BOARD_TYPE_SHELLY1)
      app.powerLedOn();
      digitalWrite( RELAY_PIN, 1 );
    #endif

    openHabHandler.sendValue("ON");
    mqttHandler.sendValue( appcfg.mqtt_outtopic, "ON");
    alexaHandler.sendValue(true);
    syslog.logInfo( "power on" );
  }
}

void RelayHandler::off()
{
  LOG0( "Switch power OFF\n" );

  if( powerOn == true )
  {
    powerOn = false;

    #if defined(BOARD_TYPE_OBI_V1)
      app.showLeds();
      digitalWrite( RELAY_TRIGGER_OFF, 0 );
      delay(5);
      digitalWrite( RELAY_TRIGGER_OFF, 1 );
    #endif

    #if defined(BOARD_TYPE_OBI_V2) || defined(BOARD_TYPE_DEV1) || defined(BOARD_TYPE_BW_SHP6) || defined(BOARD_TYPE_SHELLY1)
      app.powerLedOff();
      digitalWrite( RELAY_PIN, LOW );
    #endif

    openHabHandler.sendValue("OFF");
    mqttHandler.sendValue( appcfg.mqtt_outtopic, "OFF");
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

const bool RelayHandler::isDelayedPowerOn()
{

  bool p = powerOn;

  if( delayAction == true )
  {
    p = delayState;
  }

  // LOG1( "delay state %d %d %d\n", powerOn, delayAction, delayState );

  return p;
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
