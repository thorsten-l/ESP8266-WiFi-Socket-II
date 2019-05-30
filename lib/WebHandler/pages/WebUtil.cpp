#include "pages/Pages.h"

void prLegend(AsyncResponseStream *response, const char *name)
{
  response->printf("<legend>%s</legend>", name);
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

void prGroupLabel( AsyncResponseStream *response, int id, const char *label )
{
  response->printf(
    "<div class='pure-control-group'>"
      "<label for='pgid%d'>%s</label>", id, label );
}

void prTextGroupReadOnly( AsyncResponseStream *response, int id, const char *label,
  const char *value )
{
  prGroupLabel( response, id, label );
  response->printf(
      "<input id='pgid%d' type='text' maxlength='64' readonly value='%s'>"
    "</div>", id, value );
}
