#include <Arduino.h>
#include <App.hpp>
#include <fauxmoESP.h>
#include <AlexaHandler.hpp>
#include <OpenHabHandler.hpp>
#include <MqttHandler.hpp>
#include <OtaHandler.hpp>
#include <RelayHandler.hpp>
#include <WebHandler.hpp>
#include <WifiHandler.hpp>
#include <Hlw8012Handler.hpp>

unsigned long lifeTicker;
unsigned long maxLoopTime;
unsigned long lastLoopTimestamp;
unsigned long thisLoopTimestamp;
volatile unsigned long debounceTimestamp;
volatile unsigned long buttonTimestamp;
volatile bool buttonPressed;
volatile bool lastButtonState;

#define DEBOUNCE_TIME 100

void ICACHE_RAM_ATTR powerButtonPressed()
{
  if ((millis() - debounceTimestamp > DEBOUNCE_TIME)) // button debouncing
  {
    buttonPressed = true;
    LOG0("Button pressed\n");
  }

  debounceTimestamp = millis();
}

void setup()
{
  buttonPressed = false;
  app.setup();
  app.writeConfig();
  app.printConfig(appcfg);
  wifiHandler.setup();

#ifndef POWER_BUTTON_IS_MULTIMODE
  attachInterrupt(digitalPinToInterrupt(POWER_BUTTON), &powerButtonPressed,
                  FALLING);
#else
  lastButtonState = digitalRead( POWER_BUTTON );
  attachInterrupt(digitalPinToInterrupt(POWER_BUTTON), &powerButtonPressed,
                  CHANGE);
#endif


#ifdef HAVE_HLW8012
  hlw8012Handler.setup();
#endif

  maxLoopTime = 0l;
  lifeTicker = lastLoopTimestamp = millis();
}

void loop()
{
  thisLoopTimestamp = millis();
  maxLoopTime = max(maxLoopTime, thisLoopTimestamp - lastLoopTimestamp);
  lastLoopTimestamp = thisLoopTimestamp;

#ifndef POWER_BUTTON_IS_MULTIMODE
  if (buttonPressed)
  {
    buttonPressed = false;
    relayHandler.toggle();
  }
#else
  if (buttonPressed)
  {
    buttonPressed = false;
    bool currentButtonState = digitalRead( POWER_BUTTON );

    if( lastButtonState != currentButtonState)
    {
      lastButtonState = currentButtonState;
      // Serial.print( "-> X <- " );
      // Serial.println( currentButtonState );

      switch ( appcfg.power_button_mode )
      {
        case POWER_BUTTON_MODE_SWITCH:
          if ( currentButtonState == true )
          {
            relayHandler.delayedOn();
          }
          else
          {
            relayHandler.delayedOff();
          }
          break;

        case POWER_BUTTON_MODE_TOGGLE:
          if ( currentButtonState == true )
          {
            relayHandler.toggle();
          }
          break;

        case POWER_BUTTON_MODE_TOGGLE_SWITCH:
          relayHandler.toggle();
          break;

        default:
          LOG0( "ERROR: Unknown button mode." );
      }
    }
  } 
#endif

  if ((thisLoopTimestamp - lifeTicker) >= 10000)
  {
    LOG1("max loop time = %ld\n", maxLoopTime);
    LOG1("wifi is connected %d\n", wifiHandler.isConnected());
    LOG1("free heap %d\n", ESP.getFreeHeap());
    maxLoopTime = 0l;
    lifeTicker = thisLoopTimestamp;
  }

  if (wifiHandler.handle(thisLoopTimestamp))
  {
    otaHandler.handle();
    webHandler.handle();
    alexaHandler.handle();
    openHabHandler.handle(thisLoopTimestamp);
    mqttHandler.handle(thisLoopTimestamp);
  }

#ifdef HAVE_HLW8012
  hlw8012Handler.handle(thisLoopTimestamp);
#endif

  relayHandler.handle();
  app.handle(thisLoopTimestamp);
}
