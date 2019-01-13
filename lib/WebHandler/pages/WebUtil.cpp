#include "pages/Pages.h"

void prLegend(AsyncResponseStream *response, const char *name)
{
  response->printf("<legend>%s</legend>", name);
}

void handleCssFile(AsyncWebServerRequest *request, const uint8_t *data, size_t length)
{
  AsyncWebServerResponse *response = request->beginResponse_P(
      200, "text/css", data, length);
  response->addHeader("Content-Encoding", "gzip");
  request->send(response);
}
