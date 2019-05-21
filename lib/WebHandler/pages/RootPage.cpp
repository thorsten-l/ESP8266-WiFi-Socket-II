#include <App.hpp>
#include "pages/Pages.h"
#include <RelayHandler.hpp>
#ifdef HAVE_HLW8012
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

#ifdef HAVE_ENERGY_SENSOR
  prLegend(response, "Energy Sensor");
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

#ifdef HAVE_ENERGY_SENSOR
  response->print( "<script>function getPowerState(){"
    "var v=document.getElementById('pgid0'),"
        "c=document.getElementById('pgid1'),"
        "p=document.getElementById('pgid2');"
        "s=document.getElementById('idpwr');"
    "fetch('/state').then(resp=>resp.json()).then(function(o){"
      "v.value=o.voltage.toFixed(1)+'V',"
      "c.value=o.current.toFixed(2)+'A',"
      "p.value=o.power.toFixed(1)+'W';"
      "if(o.state===1){"
         "s.textContent=\"Power is ON\";"
         "s.style=\"background-color: #80ff80\";"
      "} else { "
         "s.textContent=\"Power is OFF\";"
         "s.style=\"background-color: #ff8080\";"
      "}"
    "})}setInterval(getPowerState,5e3);</script>" );
#else
  response->print(
      "<script>function getPowerState(){var e = "
      "document.getElementById(\"idpwr\");fetch(\"/state\").then((resp) => "
      "resp.json()).then(function(data){if(data.state===1){e.textContent="
      "\"Power is ON\";e.style=\"background-color: #80ff80\";} else "
      "{e.textContent=\"Power is OFF\";e.style=\"background-color: "
      "#ff8080\";}});} setInterval(getPowerState,10000);</script>");
#endif

  response->print(TEMPLATE_FOOTER);
  request->send(response);
}
