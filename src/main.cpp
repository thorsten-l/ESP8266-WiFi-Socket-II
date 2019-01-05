#include <Arduino.h>
#include <App.hpp>
#include <fauxmoESP.h>
#include <AlexaHandler.hpp>
#include <MqttHandler.hpp>
#include <OtaHandler.hpp>
#include <RelayHandler.hpp>
#include <WebHandler.hpp>
#include <WifiHandler.hpp>

time_t lifeTicker;
time_t maxLoopTime;
time_t lastLoopTimestamp;
time_t thisLoopTimestamp;
time_t debounceTimestamp;
bool buttonPressed;

void powerButtonPressed()
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
  app.printConfig();
  wifiHandler.setup();

  attachInterrupt(digitalPinToInterrupt(POWER_BUTTON), &powerButtonPressed,
                  FALLING);

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
    mqttHandler.handle();
  }

  relayHandler.handle();
  app.handle();
}
