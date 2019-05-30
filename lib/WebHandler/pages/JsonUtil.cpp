#include <App.hpp>
#include "pages/Pages.h"
#include <RelayHandler.hpp>
#include <Hlw8012Handler.hpp>
#include <WifiHandler.hpp>
  
static char msgBuffer[2048];

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

#if defined(HAVE_ENERGY_SENSOR) && defined(HAVE_HLW8012)
  char ebuffer[256];
  if ( powerIsOn )
  {
    sprintf( ebuffer, ",\"voltage\":%.2f,\"current\":%.2f,\"power\":%.2f", 
      hlw8012Handler.getVoltage(), hlw8012Handler.getCurrent(), 
      hlw8012Handler.getPower()
    );
  }
  else
  {
    sprintf( ebuffer, ",\"voltage\":%.2f,\"current\":0.0,\"power\":0.0", 
      hlw8012Handler.getVoltage()
    );
  }
  String m2(ebuffer);
  message += m2;
#endif

  message += "}\r\n";

  AsyncWebServerResponse *response =
      request->beginResponse(200, "application/json", message);
  response->addHeader("Access-Control-Allow-Origin", "*");
  response->addHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  response->addHeader("Pragma", "no-cache");
  response->addHeader("Expires", "0");
  request->send(response);
}

void handleJsonInfo(AsyncWebServerRequest *request)
{
  sprintf(msgBuffer,
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

          "\"wifi_ssid\":\"%s\","
          "\"wifi_reconnect_counter\":%d,"
          "\"wifi_channel\":%d,"
          "\"wifi_phy_mode\":\"%s\","
          "\"wifi_mac_address\":\"%s\","
          "\"wifi_hostname\":\"%s\","
          "\"wifi_ip_address\":\"%s\","
          "\"wifi_gateway_ip\":\"%s\","
          "\"wifi_subnet_mask\":\"%s\","
          "\"wifi_dns_ip\":\"%s\","

          "\"spiffs_total\":%u,"
          "\"spiffs_used\":%u,"
          "\"free_heap\":%u,"
          "\"sketch_size\":%u,"
          "\"free_sketch_space\":%u"
          "}",
          appcfg.ota_hostname, 
          PIOENV_NAME, 

          ESP.getFullVersion().c_str(), 
          ESP.getCoreVersion().c_str(), 
          ESP.getSdkVersion(), 

          ESP.getChipId(),
          ESP.getCpuFreqMHz(), ESP.getFlashChipRealSize(),
          ESP.getFlashChipSpeed(), ESP.getFlashChipSize(), APP_NAME,
          APP_VERSION, __DATE__, __TIME__, 
          
          appcfg.wifi_ssid,
          wifiHandler.getConnectCounter(),
          WiFi.channel(),
          wifiHandler.getPhyMode(),
          wifiHandler.getMacAddress(),
          WiFi.hostname().c_str(),
          WiFi.localIP().toString().c_str(),
          WiFi.gatewayIP().toString().c_str(),
          WiFi.subnetMask().toString().c_str(),
          WiFi.dnsIP().toString().c_str(),

          app.fsTotalBytes, app.fsUsedBytes,
          ESP.getFreeHeap(), ESP.getSketchSize(), ESP.getFreeSketchSpace()
          );

  String message(msgBuffer);

  AsyncWebServerResponse *response =
      request->beginResponse(200, "application/json", message);

  response->addHeader("Access-Control-Allow-Origin", "*");
  response->addHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  response->addHeader("Pragma", "no-cache");
  response->addHeader("Expires", "0");

  request->send(response);
}
