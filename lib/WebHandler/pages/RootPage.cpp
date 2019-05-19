#include "pages/Pages.h"
#include <RelayHandler.hpp>
#ifdef BW_SHP6
#include <Hlw8012Handler.hpp>
#endif

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

  response->print("<form class='pure-form pure-form-aligned'><fieldset>");
  prLegend(response, "Current Status");

  response->printf("<div id='idpwr' class='pure-button' "
                   "style='background-color: #%s'>Power is %s</div>",
                   webPowerState ? "80ff80" : "ff8080",
                   webPowerState ? "ON" : "OFF");

  prLegend(response, "Actions");
  response->print(
      "<a href=\"/?power=ON\" class=\"pure-button button-on\">ON</a>"
      "<a href=\"/?power=OFF\" class=\"pure-button button-off\">OFF</a>");

#ifdef BW_SHP6
  prLegend(response, "Power");
  int rid=0;
  char valueBuffer[32];
  sprintf( valueBuffer, "%0.1fV", hlw8012Handler.getVoltage() );
  prTextGroupReadOnly( response, rid++, "Voltage", valueBuffer );
  sprintf( valueBuffer, "%0.2fA", hlw8012Handler.getCurrent() );
  prTextGroupReadOnly( response, rid++, "Current", valueBuffer );
  sprintf( valueBuffer, "%0.1fW", hlw8012Handler.getPower() );
  prTextGroupReadOnly( response, rid++, "Power", valueBuffer );
#endif

  response->print("</fieldset></form>");

  response->print(
      "<script>function getPowerState(){var e = "
      "document.getElementById(\"idpwr\");fetch(\"/state\").then((resp) => "
      "resp.json()).then(function(data){if(data.state===1){e.textContent="
      "\"Power is ON\";e.style=\"background-color: #80ff80\";} else "
      "{e.textContent=\"Power is OFF\";e.style=\"background-color: "
      "#ff8080\";}});} setInterval(getPowerState,10000);</script>");

#ifdef BW_SHP6
  response->print( "<script>function getPowerState2(){var e=document.getElementById('pgid0'),t=document.getElementById('pgid1'),n=document.getElementById('pgid2');fetch('/state').then(resp=>resp.json()).then(function(o){e.value=o.voltage_v.toFixed(1)+'V',t.value=o.current_a.toFixed(2)+'A',n.value=o.power_va.toFixed(1)+'W'})}setInterval(getPowerState2,5e3);</script>" );
#endif

  response->print(TEMPLATE_FOOTER);
  request->send(response);
}
