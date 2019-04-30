#include "pages/Pages.h"


void handleResetFirmware(AsyncWebServerRequest *request)
{
  if (!request->authenticate("admin", appcfg.admin_password))
  {
    return request->requestAuthentication();
  }

  bool doReset = paramBool( request, "doreset" );

  AsyncResponseStream *response = request->beginResponseStream("text/html");
  response->printf(TEMPLATE_HEADER, APP_NAME " - Firmware Reset");
  response->addHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  response->addHeader("Pragma", "no-cache");
  response->addHeader("Expires", "0");

  if( doReset == true )
  {
    response->print(F("<h3>"));
    response->print(F("Resetting firmware... restart in about 15sec."));
    response->print(F("</h3>"));
    app.defaultConfig();
    app.delayedSystemRestart();
  }
  else
  {
    response->print(F("If you really want to reset to system defaults, you must select 'Yes' in the setup page."));
  }
  
  response->print(TEMPLATE_FOOTER);
  request->send(response);
}
