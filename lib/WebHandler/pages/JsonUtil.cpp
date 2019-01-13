#include "pages/Pages.h"
#include <RelayHandler.hpp>

void handleJsonStatus(AsyncWebServerRequest *request, int json_state)
{
  String message = F("{\"state\":");

  switch (json_state)
  {
  case JSON_RELAY_ON:
    message += "1";
    break;

  case JSON_RELAY_OFF:
    message += "0";
    break;

  default:
    message += (relayHandler.isPowerOn()) ? "1" : "0";
  };

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
          "\"free_heap\":%u"
          "}",
          appcfg.ota_hostname, PIOENV_NAME, ESP.getChipId(),
          ESP.getCpuFreqMHz(), ESP.getFlashChipRealSize(),
          ESP.getFlashChipSpeed(), ESP.getFlashChipSize(), APP_NAME,
          APP_VERSION, __DATE__, __TIME__, app.fsTotalBytes, app.fsUsedBytes,
          ESP.getFreeHeap());
  String message(buffer);

  AsyncWebServerResponse *response =
      request->beginResponse(200, "application/json", message);
  response->addHeader("Access-Control-Allow-Origin", "*");
  request->send(response);
}
