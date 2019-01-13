#ifndef __WEB_UTIL_H__
#define __WEB_UTIL_H__

#include "pages/Pages.h"

void prLegend(AsyncResponseStream *response, const char *name)
{
  response->printf("<legend>%s</legend>", name);
}

#endif

