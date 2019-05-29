#include "pages/Pages.h"
#include <FS.h>

void handleBackupConfiguration(AsyncWebServerRequest *request)
{
  if (!request->authenticate("admin", appcfg.admin_password))
  {
    return request->requestAuthentication();
  }
  
  SPIFFS.begin();
  AsyncWebServerResponse *response = request->beginResponse( SPIFFS, APP_CONFIG_FILE_JSON );
  response->setContentType("application/json");
  request->send(response);
  SPIFFS.end();
}
