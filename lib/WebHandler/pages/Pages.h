#ifndef __PAGES_H__
#define __PAGES_H__

#include <App.hpp>
#include <DefaultAppConfig.h>
#include <ESPAsyncWebServer.h>

#include "template-html.h"

#define JSON_RELAY_ON    1
#define JSON_RELAY_OFF   2
#define JSON_RELAY_STATE 3

void prLegend(AsyncResponseStream *response, const char *name);

void handleRootPage(AsyncWebServerRequest *request);
void handleInfoPage(AsyncWebServerRequest *request);
void handleSetupPage(AsyncWebServerRequest *request);
void handleSavePage(AsyncWebServerRequest *request);

void handleJsonStatus(AsyncWebServerRequest *request, int json_state );
void handleJsonInfo(AsyncWebServerRequest *request );

#endif
