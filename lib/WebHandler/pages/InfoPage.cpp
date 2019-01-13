#include "pages/Pages.h"

void handleInfoPage(AsyncWebServerRequest *request)
{
  AsyncResponseStream *response = request->beginResponseStream("text/html");
    response->printf(TEMPLATE_HEADER, APP_NAME " - Info");

    response->print("<form class='pure-form'>");

    prLegend(response, "Application");

    response->print(
        "<p>Name: " APP_NAME "</p>"
        "<p>Version: " APP_VERSION "</p>"
        "<p>PlatformIO Environment: " PIOENV_NAME "</p>"
        "<p>Author: Dr. Thorsten Ludewig &lt;t.ludewig@gmail.com></p>" );

    prLegend(response, "RESTful API");

    char ipAddress[16];
    strncpy(ipAddress, appcfg.net_host, 15);
    ipAddress[15] = 0;

    response->printf(
        "<p><a href='http://%s/on'>http://%s/on</a> - Socket ON</p>"
        "<p><a href='http://%s/off'>http://%s/off</a> - Socket OFF</p>"
        "<p><a href='http://%s/state'>http://%s/state</a> - Socket JSON status "
        "(0 or 1)</p>"
        "<p><a href='http://%s/info'>http://%s/info</a> - ESP8266 Info</p>",
        ipAddress, ipAddress, ipAddress, ipAddress, ipAddress, ipAddress,
        ipAddress, ipAddress);

    prLegend(response, "Build");
    response->print("<p>Date: " __DATE__ "</p>"
                    "<p>Time: " __TIME__ "</p>");

    prLegend(response, "Services");
    response->printf("<p>OpenHAB Callback Enabled: %s</p>",
                     (appcfg.ohab_enabled) ? "true" : "false");
    response->printf("<p>Alexa Enabled: %s</p>",
                     (appcfg.alexa_enabled) ? "true" : "false");
    response->printf("<p>MQTT Enabled: %s</p>",
                     (appcfg.mqtt_enabled) ? "true" : "false");
    response->printf("<p>Syslog Enabled: %s</p>",
                     (appcfg.syslog_enabled) ? "true" : "false");

    response->print("</form>");
    response->print(TEMPLATE_FOOTER);
    request->send(response);
}
