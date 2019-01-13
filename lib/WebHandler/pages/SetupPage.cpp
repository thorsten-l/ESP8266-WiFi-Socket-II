#include "pages/Pages.h"
#include "setup-html.h"
#include <LinkedList.hpp>
#include <WifiHandler.hpp>

String setupProcessor(const String &var)
{
  String selected = F("selected");
  String checked = F("checked");

  if (var == "admin_password")
    return String(appcfg.admin_password);

  // Wifi
  if (var == "wifi_mode_ap" && appcfg.wifi_mode == WIFI_AP)
    return selected;
  if (var == "wifi_mode_station" && appcfg.wifi_mode == WIFI_STA)
    return selected;

  if (var == "scanned_network_options")
  {
    ListNode *node = wifiHandler.getScannedNetworks();
    String options = "";

    while (node != NULL)
    {
      options += F("<option>");
      options += String(*node->value);
      options += F("</option>");
      node = node->next;
    }

    return options;
  }

  if (var == "wifi_ssid")
    return String(appcfg.wifi_ssid);
  if (var == "wifi_password")
    return String(appcfg.wifi_password);

  // Network
  if (var == "net_mode_dhcp" && appcfg.net_mode == NET_MODE_DHCP)
    return selected;
  if (var == "net_mode_static" && appcfg.net_mode == NET_MODE_STATIC)
    return selected;
  if (var == "net_host")
    return String(appcfg.net_host);
  if (var == "net_gateway")
    return String(appcfg.net_gateway);
  if (var == "net_mask")
    return String(appcfg.net_mask);
  if (var == "net_dns")
    return String(appcfg.net_dns);

  // OTA
  if (var == "ota_hostname")
    return String(appcfg.ota_hostname);
  if (var == "ota_password")
    return String(appcfg.ota_password);

  // OpenHAB
  if (var == "ohab_enabled" && appcfg.ohab_enabled == true)
    return checked;
  if (var == "ohab_v1" && appcfg.ohab_version == 1)
    return selected;
  if (var == "ohab_v2" && appcfg.ohab_version == 2)
    return selected;
  if (var == "ohab_itemname")
    return String(appcfg.ohab_itemname);
  if (var == "ohab_host")
    return String(appcfg.ohab_host);
  if (var == "ohab_port")
    return String(appcfg.ohab_port);
  if (var == "ohab_useauth" && appcfg.ohab_useauth == true)
    return checked;
  if (var == "ohab_user")
    return String(appcfg.ohab_user);
  if (var == "ohab_password")
    return String(appcfg.ohab_password);

  // Alexa
  if (var == "alexa_enabled" && appcfg.alexa_enabled == true)
    return checked;
  if (var == "alexa_devicename")
    return String(appcfg.alexa_devicename);

  // MQTT
  if (var == "mqtt_enabled" && appcfg.mqtt_enabled == true)
    return checked;
  if (var == "mqtt_clientid")
    return String(appcfg.mqtt_clientid);
  if (var == "mqtt_host")
    return String(appcfg.mqtt_host);
  if (var == "mqtt_port")
    return String(appcfg.mqtt_port);
  if (var == "mqtt_useauth" && appcfg.mqtt_useauth == true)
    return checked;
  if (var == "mqtt_user")
    return String(appcfg.mqtt_user);
  if (var == "mqtt_password")
    return String(appcfg.mqtt_password);
  if (var == "mqtt_intopic")
    return String(appcfg.mqtt_intopic);
  if (var == "mqtt_outtopic")
    return String(appcfg.mqtt_outtopic);

  // Syslog
  if (var == "syslog_enabled" && appcfg.syslog_enabled == true)
    return checked;
  if (var == "syslog_host")
    return String(appcfg.syslog_host);
  if (var == "syslog_port")
    return String(appcfg.syslog_port);
  if (var == "syslog_app_name")
    return String(appcfg.syslog_app_name);

  return String();
}

void handleSetupPage(AsyncWebServerRequest *request)
{
  if (!request->authenticate("admin", appcfg.admin_password))
  {
    return request->requestAuthentication();
  }

  AsyncWebServerResponse *response =
      request->beginResponse_P(200, "text/html", SETUP_HTML, setupProcessor);
  response->addHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  response->addHeader("Pragma", "no-cache");
  response->addHeader("Expires", "0");
  request->send(response);
}
