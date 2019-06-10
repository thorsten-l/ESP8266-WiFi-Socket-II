#include "pages/Pages.h"

static bool updateSucceed;
static int alter;
static bool validFilename;
static char filenameBuffer[256];

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
    Serial.print( "Filename: ");
    Serial.println( filename );

    if( filename.endsWith( String( "." PIOENV_NAME ".bin" )) == true )
    {
      validFilename = true;
    }
    else
    {
      validFilename = false;
      strcpy( filenameBuffer, filename.c_str() );
      Serial.println( "ERROR: invalid filename" );
    }
    
    updateSucceed = false;
    alter = 0;

    if ( validFilename == true )
    { 
      Update.runAsync(true);
      if (!Update.begin(free_space))
      {
        Update.printError(Serial);
      }
    }
  }
  else
  {
    if ( validFilename == true )
    { 
      Serial.printf("\rprogress: %u", Update.progress());
      alter ^= 1;
#ifdef WIFI_LED
      digitalWrite( WIFI_LED, alter );
#endif
    }
  }

  if ( validFilename == true )
  {
    if ( Update.write(data, len) != len)
    {
      Serial.println();
      Update.printError(Serial);
    }

    if (final)
    {
      Serial.println();

      if (!Update.end(true))
      {
#ifdef WIFI_LED
        digitalWrite( WIFI_LED, WIFI_LED_ON );
#endif
        Update.printError(Serial);
      }
      else
      {
#ifdef WIFI_LED
        digitalWrite( WIFI_LED, WIFI_LED_OFF );
#endif
        updateSucceed = true;
        app.delayedSystemRestart();
        Serial.println("Update complete");
      }
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
  response->print(TEMPLATE_HEADER);
  response->print(META_REFRESH);
  response->printf(TEMPLATE_BODY, APP_NAME " - Firmware Update");

  response->print(F("<h3>"));

  if( updateSucceed )
  {
    response->print(F("Upload succeed... restart in about 30sec."));
  }
  else
  {
    response->print(F("ERROR: Upload failed!<br/>"));
  }

  response->print(F("</h3>"));

  if ( validFilename == false )
  {
    response->print(F("Invalid firmware filename: <tt>"));
    response->print( filenameBuffer );
    response->print(F("</tt><br/>firmware filename must ends with: '<tt>." PIOENV_NAME ".bin</tt>'" ));
  }

  response->print(TEMPLATE_FOOTER);
  request->send(response);
}
