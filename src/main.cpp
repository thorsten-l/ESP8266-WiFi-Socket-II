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
unsigned long debounceTimestamp;
bool buttonPressed;

void ICACHE_RAM_ATTR powerButtonPressed()
{
  if ((millis() - debounceTimestamp > 500)) // button debouncing
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

  attachInterrupt(digitalPinToInterrupt(POWER_BUTTON), &powerButtonPressed,
                  FALLING);

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

  if (buttonPressed)
  {
    buttonPressed = false;
    relayHandler.toggle();
  }

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
