#include "WebHandler.hpp"
#include "pages/Pages.h"
#include "favicon_ico.h"
#include <ESP8266mDNS.h>
#include <AlexaHandler.hpp>
#include <RelayHandler.hpp>

WebHandler webHandler;
static AsyncWebServer server(80);

void captivePortal(AsyncWebServerRequest *request)
{
  request->redirect("http://wifi.socket/captive.html");
}

WebHandler::WebHandler() 
{ 
  initialized = false; 
}

void WebHandler::setup()
{
  LOG0("HTTP server setup...\n");

  server.on("/", HTTP_GET, handleRootPage);
  server.on("/setup.html", HTTP_GET, handleSetupPage );
  server.on("/maintenance.html", HTTP_GET, handleMaintenanceSetupPage );
  server.on("/info.html", HTTP_GET, handleInfoPage );
  server.on("/savecfg", HTTP_POST, handleSavePage);
  server.on("/info", HTTP_GET, handleJsonInfo );
  server.on("/update-firmware", HTTP_POST, handleUpdateFirmware, handleUpdateProgressCB );
  server.on("/reset-firmware", HTTP_POST, handleResetFirmware );

  server.on("/on", HTTP_GET, [](AsyncWebServerRequest *request) {
    relayHandler.delayedOn();
    handleJsonStatus( request, JSON_RELAY_ON );
  });

  if ( appcfg.wifi_mode == WIFI_AP )
  {
    server.on("/captive.html", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->send(200, "text/html", 
        F("Please enter <b><a target='_blank' href='http://wifi.socket'>http://wifi.socket</a></b> or <b><a target='_blank' href='http://192.168.192.1'>http://192.168.192.1</a></b> into your browser."));
    });
  }

  server.on("/off", HTTP_GET, [](AsyncWebServerRequest *request) {
    relayHandler.delayedOff();
    handleJsonStatus( request, JSON_RELAY_OFF );
  });

  server.on("/state", HTTP_GET, [](AsyncWebServerRequest *request) {
    handleJsonStatus( request, JSON_RELAY_STATE );
  });

  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response = request->beginResponse_P(200, "image/x-icon", FAVICON_ICO_GZ, FAVICON_ICO_GZ_LEN );
    response->addHeader("Content-Encoding", "gzip");
    response->addHeader("Cache-Control", "max-age=31536000");
    request->send(response);
  });

  server.on("/pure-min.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/css", PURE_MIN_CSS_GZ, PURE_MIN_CSS_GZ_LEN);
    response->addHeader("Content-Encoding", "gzip");
    response->addHeader("Cache-Control", "max-age=31536000");
    request->send(response);
  });

  server.on("/layout.css", HTTP_GET, [](AsyncWebServerRequest *request) {
    AsyncWebServerResponse *response = request->beginResponse_P(200, "text/css", LAYOUT_CSS_GZ, LAYOUT_CSS_GZ_LEN);
    response->addHeader("Content-Encoding", "gzip");
    response->addHeader("Cache-Control", "max-age=31536000");
    request->send(response);
  });

  server.on("/config-backup", HTTP_GET, handleBackupConfiguration );
  server.on("/config-restore", HTTP_POST, handleRestoreConfiguration, handleRestoreConfigurationCB );

  if (appcfg.alexa_enabled == true)
  {
    // These two callbacks are required for gen1 and gen3 compatibility
    server.onRequestBody([](AsyncWebServerRequest *request, uint8_t *data,
                            size_t len, size_t index, size_t total) {
      LOG0("server.onRequestBody ");
      if (fauxmo.process(request->client(), request->method() == HTTP_GET,
                         request->url(), String((char *)data)))
        return;
    });
    
    server.onNotFound([](AsyncWebServerRequest *request) {
      String body = (request->hasParam("body", true))
                        ? request->getParam("body", true)->value()
                        : String();
      if (fauxmo.process(request->client(), request->method() == HTTP_GET,
                         request->url(), body))
        return;
      request->send(404);
    });
  }
  else
  {
    if ( appcfg.wifi_mode == WIFI_AP )
    {
      server.onNotFound( captivePortal );
    }
    else
    {
      server.onNotFound([](AsyncWebServerRequest *request) { request->send(404); });
    }  
  }

  MDNS.addService("http", "tcp", 80);
  MDNS.addServiceTxt("http", "tcp", "path", "/");
  MDNS.addServiceTxt("http", "tcp", "fw_name", APP_NAME);
  MDNS.addServiceTxt("http", "tcp", "fw_version", APP_VERSION);

  server.begin();

  LOG0("HTTP server started\n");
  initialized = true;
}

void WebHandler::handle()
{
  if (!initialized)
  {
    setup();
  }

  MDNS.update();
}
