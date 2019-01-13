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

bool paramBool(AsyncWebServerRequest *request, const char *paramName)
{
  bool value = false;

  if (request->hasParam(paramName, true))
  {
    AsyncWebParameter *p = request->getParam(paramName, true);
    const char *pv = p->value().c_str();
    if (pv != 0 && strlen(pv) > 0)
    {
      value = strcmp("true", pv) == 0;
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

  AsyncResponseStream *response = request->beginResponseStream("text/html");
  response->printf(TEMPLATE_HEADER, APP_NAME " - Save Configuration");
  response->print("<pre>");

  int params = request->params();

  for (int i = 0; i < params; i++)
  {
    AsyncWebParameter *p = request->getParam(i);
    response->printf("%s = '%s'\n", p->name().c_str(), p->value().c_str());
  }

  // Security
  paramChars(request, appcfgWR.admin_password, "admin_password",
             DEFAULT_ADMIN_PASSWORD);

  // WIFI
  appcfgWR.wifi_mode = paramInt(request, "wifi_mode", DEFAULT_WIFI_MODE);
  paramChars(request, appcfgWR.wifi_ssid, "wifi_ssid", DEFAULT_WIFI_SSID);
  paramChars(request, appcfgWR.wifi_password, "wifi_password",
             DEFAULT_WIFI_PASSWORD);

  // Network
  appcfgWR.net_mode = paramInt(request, "net_mode", DEFAULT_NET_MODE);
  paramChars(request, appcfgWR.net_host, "net_host", DEFAULT_NET_HOST);
  paramChars(request, appcfgWR.net_gateway, "net_gateway", DEFAULT_NET_GATEWAY);
  paramChars(request, appcfgWR.net_mask, "net_mask", DEFAULT_NET_MASK);
  paramChars(request, appcfgWR.net_dns, "net_dns", DEFAULT_NET_DNS);

  // OTA
  paramChars(request, appcfgWR.ota_hostname, "ota_hostname",
             DEFAULT_OTA_HOSTNAME);
  paramChars(request, appcfgWR.ota_password, "ota_password",
             DEFAULT_OTA_PASSWORD);

  // OpenHAB
  appcfgWR.ohab_enabled = paramBool(request, "ohab_enabled");
  appcfgWR.ohab_version =
      paramInt(request, "ohab_version", DEFAULT_OHAB_VERSION);
  paramChars(request, appcfgWR.ohab_itemname, "ohab_itemname",
             DEFAULT_OHAB_ITEMNAME);
  paramChars(request, appcfgWR.ohab_host, "ohab_host", DEFAULT_OHAB_HOST);
  appcfgWR.ohab_port = paramInt(request, "ohab_port", DEFAULT_OHAB_PORT);
  appcfgWR.ohab_useauth = paramBool(request, "ohab_useauth");
  paramChars(request, appcfgWR.ohab_user, "ohab_user", DEFAULT_OHAB_USER);
  paramChars(request, appcfgWR.ohab_password, "ohab_password",
             DEFAULT_OHAB_PASSWORD);

  // Alexa
  appcfgWR.alexa_enabled = paramBool(request, "alexa_enabled");
  paramChars(request, appcfgWR.alexa_devicename, "alexa_devicename",
             DEFAULT_ALEXA_DEVICENAME);

  // MQTT
  appcfgWR.mqtt_enabled = paramBool(request, "mqtt_enabled");
  paramChars(request, appcfgWR.mqtt_clientid, "mqtt_clientid",
             DEFAULT_MQTT_CLIENTID);
  paramChars(request, appcfgWR.mqtt_host, "mqtt_host", DEFAULT_MQTT_HOST);
  appcfgWR.mqtt_port = paramInt(request, "mqtt_port", DEFAULT_MQTT_PORT);
  appcfgWR.mqtt_useauth = paramBool(request, "mqtt_useauth");
  paramChars(request, appcfgWR.mqtt_user, "mqtt_user", DEFAULT_MQTT_USER);
  paramChars(request, appcfgWR.mqtt_password, "mqtt_password",
             DEFAULT_MQTT_PASSWORD);
  paramChars(request, appcfgWR.mqtt_intopic, "mqtt_intopic",
             DEFAULT_MQTT_INTOPIC);
  paramChars(request, appcfgWR.mqtt_outtopic, "mqtt_outtopic",
             DEFAULT_MQTT_OUTTOPIC);

  // Syslog
  appcfgWR.syslog_enabled = paramBool(request, "syslog_enabled");
  paramChars(request, appcfgWR.syslog_host, "syslog_host", DEFAULT_SYSLOG_HOST);
  appcfgWR.syslog_port = paramInt(request, "syslog_port", DEFAULT_SYSLOG_PORT);
  paramChars(request, appcfgWR.syslog_app_name, "syslog_app_name",
             DEFAULT_SYSLOG_APP_NAME);

  response->println("</pre>");
  response->println("<h2 style='color: red'>Restarting System</h2>");
  response->print(TEMPLATE_FOOTER);
  request->send(response);
  app.delayedSystemRestart();
}
