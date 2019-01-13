#ifndef __ROOT_PAGE_H__
#define __ROOT_PAGE_H__

#include "pages/Pages.h"
#include <RelayHandler.hpp>

static bool webPowerState;

void handleRootPage(AsyncWebServerRequest *request)
{
  webPowerState = relayHandler.isPowerOn();

  if (request->hasParam("power"))
  {
    AsyncWebParameter *p = request->getParam("power");
    const char *pv = p->value().c_str();
    if (pv != 0 && strlen(pv) > 0)
    {
      if (strcmp("ON", pv) == 0)
      {
        webPowerState = true;
        relayHandler.delayedOn();
      }
      if (strcmp("OFF", pv) == 0)
      {
        webPowerState = false;
        relayHandler.delayedOff();
      }
    }
    request->redirect("/");
    return;
  }

  char titleBuffer[100];
  sprintf(titleBuffer, APP_NAME " - %s", appcfg.ota_hostname);

  AsyncResponseStream *response = request->beginResponseStream("text/html");
  response->printf(TEMPLATE_HEADER, titleBuffer);

  response->print("<form class='pure-form'>");
  prLegend(response, "Current Status");

  response->printf("<div id='idpwr' class='pure-button' "
                   "style='background-color: #%s'>Power is %s</div>",
                   webPowerState ? "80ff80" : "ff8080",
                   webPowerState ? "ON" : "OFF");
  prLegend(response, "Actions");
  response->print(
      "<a href=\"/?power=ON\" class=\"pure-button button-on\">ON</a>"
      "<a href=\"/?power=OFF\" class=\"pure-button button-off\">OFF</a>");

  response->print("</form>");

  response->print(
      "<script>function getPowerState(){var e = "
      "document.getElementById(\"idpwr\");fetch(\"/state\").then((resp) => "
      "resp.json()).then(function(data){if(data.state===1){e.textContent="
      "\"Power is ON\";e.style=\"background-color: #80ff80\";} else "
      "{e.textContent=\"Power is OFF\";e.style=\"background-color: "
      "#ff8080\";}});} setInterval(getPowerState,10000);</script>");

  response->print(TEMPLATE_FOOTER);
  request->send(response);
}

#endif
