#include "pages/Pages.h"
#include <FS.h>
#include <MicroJson.hpp>

#define TEMP_FILENAME "/config.tmp"

bool uploadSucceed;

void handleRestoreConfigurationCB(AsyncWebServerRequest *request,
                                  String filename, size_t index, uint8_t *data,
                                  size_t len, bool final) {

  if (!request->authenticate("admin", appcfg.admin_password)) {
    return request->requestAuthentication();
  }

  File tmpConfig;

  if (!index) {
    Serial.println("Restore configurtion started...");
    uploadSucceed = false;
    SPIFFS.begin();
    tmpConfig = SPIFFS.open( TEMP_FILENAME, "w" );
  }

  tmpConfig.write(data, len);

  if (final) {
    tmpConfig.close();
    Serial.println();

    if ( app.loadJsonConfig( TEMP_FILENAME ) == false )
    {
      app.printConfig(appcfgRD);
      memcpy(&appcfgWR, &appcfgRD, sizeof(appcfg));
      uploadSucceed = true;
    }

    SPIFFS.end();
    app.delayedSystemRestart();
    Serial.println("Restore configuration complete");
  }
}

void handleRestoreConfiguration(AsyncWebServerRequest *request) {
  if (!request->authenticate("admin", appcfg.admin_password)) {
    return request->requestAuthentication();
  }

  AsyncResponseStream *response = request->beginResponseStream("text/html");
  response->print(TEMPLATE_HEADER);
  response->print(META_REFRESH);
  response->printf(TEMPLATE_BODY, APP_NAME " - Restore configuration");
  response->print(F("<h3>"));

  if ( uploadSucceed == false )
  {
    response->print(F("ERROR: Restore configuration failed!"));
  }
  else
  {
    response->print(F("Restore configuration succeed... restart in about 30sec."));
  }
  
  response->print(F("</h3>"));
  response->print(TEMPLATE_FOOTER);
  request->send(response);
}
