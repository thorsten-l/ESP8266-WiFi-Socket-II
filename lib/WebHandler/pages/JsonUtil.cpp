#include <App.hpp>
#include "pages/Pages.h"
#include <RelayHandler.hpp>
#include <Hlw8012Handler.hpp>

void handleJsonStatus(AsyncWebServerRequest *request, int json_state)
{
  bool powerIsOn;

  String message = F("{\"state\":");

  switch (json_state)
  {
  case JSON_RELAY_ON:
    message += "1";
    powerIsOn = true;
    break;

  case JSON_RELAY_OFF:
    message += "0";
    powerIsOn = false;
    break;

  default:
    powerIsOn = relayHandler.isPowerOn();
    message += (powerIsOn) ? "1" : "0";
  };

#ifdef HAVE_ENERGY_SENSOR
  char buffer[256];
  if ( powerIsOn )
  {
    sprintf( buffer, ",\"voltage\":%.2f,\"current\":%.2f,\"power\":%.2f", 
      hlw8012Handler.getVoltage(), hlw8012Handler.getCurrent(), 
      hlw8012Handler.getPower()
    );
  }
  else
  {
    sprintf( buffer, ",\"voltage\":%.2f,\"current\":0.0,\"power\":0.0", 
      hlw8012Handler.getVoltage()
    );
  }
  String m2(buffer);
  message += m2;
#endif

  message += "}\r\n";

  AsyncWebServerResponse *response =
      request->beginResponse(200, "application/json", message);
  response->addHeader("Access-Control-Allow-Origin", "*");
  request->send(response);
}

void handleJsonInfo(AsyncWebServerRequest *request)
{
  char buffer[768];
  sprintf(buffer,
          "{"
          "\"host_name\":\"%s\","
          "\"pioenv_name\":\"%s\","
          "\"esp_full_version\":\"%s\","
          "\"esp_core_version\":\"%s\","
          "\"esp_sdk_version\":\"%s\","
          "\"chip_id\":\"%08X\","
          "\"cpu_freq\":\"%dMhz\","
          "\"flash_size\":%u,"
          "\"flash_speed\":%u,"
          "\"ide_size\":%u,"
          "\"fw_name\":\"%s\","
          "\"fw_version\":\"%s\","
          "\"build_date\":\"%s\","
          "\"build_time\":\"%s\","
          "\"spiffs_total\":%u,"
          "\"spiffs_used\":%u,"
          "\"free_heap\":%u,"
          "\"sketch_size\":%u,"
          "\"free_sketch_space\":%u"
          "}",
          appcfg.ota_hostname, PIOENV_NAME, 

          ESP.getFullVersion().c_str(), 
          ESP.getCoreVersion().c_str(), 
          ESP.getSdkVersion(), 

          ESP.getChipId(),
          ESP.getCpuFreqMHz(), ESP.getFlashChipRealSize(),
          ESP.getFlashChipSpeed(), ESP.getFlashChipSize(), APP_NAME,
          APP_VERSION, __DATE__, __TIME__, app.fsTotalBytes, app.fsUsedBytes,
          ESP.getFreeHeap(), ESP.getSketchSize(), ESP.getFreeSketchSpace()
          );

  String message(buffer);

  AsyncWebServerResponse *response =
      request->beginResponse(200, "application/json", message);
  response->addHeader("Access-Control-Allow-Origin", "*");
  request->send(response);
}
