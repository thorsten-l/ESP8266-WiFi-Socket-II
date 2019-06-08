#include "pages/Pages.h"

void paramChars(AsyncWebServerRequest *request, char *dest,
                const char *paramName, const char *defaultValue)
{
  const char *value = defaultValue;

  if (request->hasParam(paramName, true))
  {
    AsyncWebParameter *p = request->getParam(paramName, true);
    value = p->value().c_str();
    if (value == 0 || strlen(value) == 0)
    {
      value = defaultValue;
    }
  }

  strncpy(dest, value, 63);
  dest[63] = 0;
}

int paramInt(AsyncWebServerRequest *request, const char *paramName,
             int defaultValue)
{
  int value = defaultValue;

  if (request->hasParam(paramName, true))
  {
    AsyncWebParameter *p = request->getParam(paramName, true);
    const char *pv = p->value().c_str();
    if (pv != 0 && strlen(pv) > 0)
    {
      value = atoi(pv);
    }
  }

  return value;
}

void handleSavePage(AsyncWebServerRequest *request)
{
  if (!request->authenticate("admin", appcfg.admin_password))
  {
    return request->requestAuthentication();
  }


  // Security
  paramChars(request, appcfgWR.admin_password, A_admin_password,
             DEFAULT_ADMIN_PASSWORD);

  // WIFI
  appcfgWR.wifi_mode = paramInt(request, A_wifi_mode, DEFAULT_WIFI_MODE);
  paramChars(request, appcfgWR.wifi_ssid, A_wifi_ssid, DEFAULT_WIFI_SSID);
  paramChars(request, appcfgWR.wifi_password, A_wifi_password,
             DEFAULT_WIFI_PASSWORD);

  // Network
  appcfgWR.net_mode = paramInt(request, A_net_mode, DEFAULT_NET_MODE);
  paramChars(request, appcfgWR.net_host, A_net_host, DEFAULT_NET_HOST);
  paramChars(request, appcfgWR.net_gateway, A_net_gateway, DEFAULT_NET_GATEWAY);
  paramChars(request, appcfgWR.net_mask, A_net_mask, DEFAULT_NET_MASK);
  paramChars(request, appcfgWR.net_dns, A_net_dns, DEFAULT_NET_DNS);

  // OTA
  paramChars(request, appcfgWR.ota_hostname, A_ota_hostname,
             DEFAULT_OTA_HOSTNAME);
  paramChars(request, appcfgWR.ota_password, A_ota_password,
             DEFAULT_OTA_PASSWORD);

  // OpenHAB
  appcfgWR.ohab_enabled = paramBool(request, A_ohab_enabled);
  appcfgWR.ohab_version =
      paramInt(request, A_ohab_version, DEFAULT_OHAB_VERSION);
  paramChars(request, appcfgWR.ohab_itemname, A_ohab_itemname,
             DEFAULT_OHAB_ITEMNAME);
  paramChars(request, appcfgWR.ohab_host, A_ohab_host, DEFAULT_OHAB_HOST);
  appcfgWR.ohab_port = paramInt(request, A_ohab_port, DEFAULT_OHAB_PORT);
  appcfgWR.ohab_useauth = paramBool(request, A_ohab_useauth);
  paramChars(request, appcfgWR.ohab_user, A_ohab_user, DEFAULT_OHAB_USER);
  paramChars(request, appcfgWR.ohab_password, A_ohab_password,
             DEFAULT_OHAB_PASSWORD);

#ifdef HAVE_ENERGY_SENSOR
  paramChars(request, appcfgWR.ohab_item_voltage, A_ohab_item_voltage, DEFAULT_OHAB_ITEM_VOLTAGE);
  paramChars(request, appcfgWR.ohab_item_current, A_ohab_item_current, DEFAULT_OHAB_ITEM_CURRENT);
  paramChars(request, appcfgWR.ohab_item_power, A_ohab_item_power, DEFAULT_OHAB_ITEM_POWER);
  appcfgWR.ohab_sending_interval = paramInt(request, A_ohab_sending_interval, DEFAULT_OHAB_SENDING_INTERVAL);
#endif

  // Alexa
  appcfgWR.alexa_enabled = paramBool(request, A_alexa_enabled);
  paramChars(request, appcfgWR.alexa_devicename, A_alexa_devicename,
             DEFAULT_ALEXA_DEVICENAME);

  // MQTT
  appcfgWR.mqtt_enabled = paramBool(request, A_mqtt_enabled);
  paramChars(request, appcfgWR.mqtt_clientid, A_mqtt_clientid,
             DEFAULT_MQTT_CLIENTID);
  paramChars(request, appcfgWR.mqtt_host, A_mqtt_host, DEFAULT_MQTT_HOST);
  appcfgWR.mqtt_port = paramInt(request, A_mqtt_port, DEFAULT_MQTT_PORT);
  appcfgWR.mqtt_useauth = paramBool(request, A_mqtt_useauth);
  paramChars(request, appcfgWR.mqtt_user, A_mqtt_user, DEFAULT_MQTT_USER);
  paramChars(request, appcfgWR.mqtt_password, A_mqtt_password,
             DEFAULT_MQTT_PASSWORD);
  paramChars(request, appcfgWR.mqtt_intopic, A_mqtt_intopic,
             DEFAULT_MQTT_INTOPIC);
  paramChars(request, appcfgWR.mqtt_outtopic, A_mqtt_outtopic,
             DEFAULT_MQTT_OUTTOPIC);

#ifdef HAVE_ENERGY_SENSOR
  paramChars(request, appcfgWR.mqtt_topic_voltage, A_mqtt_topic_voltage, DEFAULT_MQTT_TOPIC_VOLTAGE);
  paramChars(request, appcfgWR.mqtt_topic_current, A_mqtt_topic_current, DEFAULT_MQTT_TOPIC_CURRENT);
  paramChars(request, appcfgWR.mqtt_topic_power, A_mqtt_topic_power, DEFAULT_MQTT_TOPIC_POWER);
  paramChars(request, appcfgWR.mqtt_topic_json, A_mqtt_topic_json, DEFAULT_MQTT_TOPIC_JSON);
  appcfgWR.mqtt_sending_interval = paramInt(request, A_mqtt_sending_interval, DEFAULT_MQTT_SENDING_INTERVAL);
#endif

  // Syslog
  appcfgWR.syslog_enabled = paramBool(request, A_syslog_enabled);
  paramChars(request, appcfgWR.syslog_host, A_syslog_host, DEFAULT_SYSLOG_HOST);
  appcfgWR.syslog_port = paramInt(request, A_syslog_port, DEFAULT_SYSLOG_PORT);
  paramChars(request, appcfgWR.syslog_app_name, A_syslog_app_name,
             DEFAULT_SYSLOG_APP_NAME);

#ifdef POWER_BUTTON_IS_MULTIMODE
  appcfgWR.power_button_mode = paramInt(request, A_power_button_mode, DEFAULT_POWER_BUTTON_MODE);
#endif

  AsyncResponseStream *response = request->beginResponseStream("text/html");
  response->print(TEMPLATE_HEADER);
  response->print(META_REFRESH);
  response->printf( TEMPLATE_BODY, APP_NAME " - Save Configuration" );
  response->println("<h2>Configuration saved.</h2>");
  response->println("<h3 style='color: red'>Restarting System ... takes about 30s</h3>");
  response->print(TEMPLATE_FOOTER);
  LOG0( "*** save config parsed ... sending response\n" );
  request->send(response);
  LOG0( "*** response send\n" );
  app.delayedSystemRestart();
}
