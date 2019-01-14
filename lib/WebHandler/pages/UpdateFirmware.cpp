#include "pages/Pages.h"

static bool updateSucceed;

void handleUpdateProgressCB(AsyncWebServerRequest *request, String filename,
                            size_t index, uint8_t *data, size_t len,
                            bool final)
{
  if (!request->authenticate("admin", appcfg.admin_password))
  {
    return request->requestAuthentication();
  }

  uint32_t free_space = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;

  if (!index)
  {
    Serial.println("Update firmware via HTTP");
    updateSucceed = false;

    Update.runAsync(true);
    if (!Update.begin(free_space))
    {
      Update.printError(Serial);
    }
  }
  else
  {
    Serial.printf("\rprogress: %u", Update.progress());
  }

  if (Update.write(data, len) != len)
  {
    Serial.println();
    Update.printError(Serial);
  }

  if (final)
  {
    Serial.println();

    if (!Update.end(true))
    {
      Update.printError(Serial);
    }
    else
    {
      updateSucceed = true;
      app.delayedSystemRestart();
      Serial.println("Update complete");
    }
  }
}

void handleUpdateFirmware(AsyncWebServerRequest *request)
{
  if (!request->authenticate("admin", appcfg.admin_password))
  {
    return request->requestAuthentication();
  }

  AsyncResponseStream *response = request->beginResponseStream("text/html");
  response->printf(TEMPLATE_HEADER, APP_NAME " - Firmware Update");

  response->print(F("<h3>"));

  if( updateSucceed )
  {
    response->print(F("Upload succeed... restart in about 5sec."));
  }
  else
  {
    response->print(F("ERROR: Upload failed!"));
  }

  response->print(F("</h3>"));

  response->print(TEMPLATE_FOOTER);
  request->send(response);
}
