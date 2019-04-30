#include "pages/Pages.h"

void prLegend(AsyncResponseStream *response, const char *name)
{
  response->printf("<legend>%s</legend>", name);
}

void handleCssFile(AsyncWebServerRequest *request, const uint8_t *data,
                   size_t length)
{
  AsyncWebServerResponse *response =
      request->beginResponse_P(200, "text/css", data, length);
  response->addHeader("Content-Encoding", "gzip");
  request->send(response);
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