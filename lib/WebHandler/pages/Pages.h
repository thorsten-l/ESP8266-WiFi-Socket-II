#ifndef __PAGES_H__
#define __PAGES_H__

#include <App.hpp>
#include <DefaultAppConfig.h>
#include <ESPAsyncWebServer.h>

#include "template-html.h"
#include "layout-css-gz.h"
#include "pure-min-css-gz.h"

#define JSON_RELAY_ON    1
#define JSON_RELAY_OFF   2
#define JSON_RELAY_STATE 3

void prGroupLabel( AsyncResponseStream *response, int id, const char *label );
void prLegend(AsyncResponseStream *response, const char *name);
bool paramBool(AsyncWebServerRequest *request, const char *paramName);
void prTextGroupReadOnly( AsyncResponseStream *response, int id, const char *label,
  const char *value );
   
void handleRootPage(AsyncWebServerRequest *request);
void handleInfoPage(AsyncWebServerRequest *request);
void handleSetupPage(AsyncWebServerRequest *request);
void handleSavePage(AsyncWebServerRequest *request);

void handleJsonStatus(AsyncWebServerRequest *request, int json_state );
void handleJsonInfo(AsyncWebServerRequest *request );
void handleBackupConfiguration(AsyncWebServerRequest *request );
void handleRestoreConfiguration(AsyncWebServerRequest *request );
void handleRestoreConfigurationCB(AsyncWebServerRequest *request, String filename,
                            size_t index, uint8_t *data, size_t len,
                            bool final);

void handleMaintenanceSetupPage(AsyncWebServerRequest *request);

void handleCssFile(AsyncWebServerRequest *request, const uint8_t *data, size_t length );

void handleUpdateFirmware(AsyncWebServerRequest *request);
void handleUpdateProgressCB(AsyncWebServerRequest *request, String filename,
                               size_t index, uint8_t *data, size_t len,
                               bool final);

void handleResetFirmware(AsyncWebServerRequest *request);

#endif
