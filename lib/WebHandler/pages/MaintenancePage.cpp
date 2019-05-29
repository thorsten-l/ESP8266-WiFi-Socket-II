#include "maintenance-html.h"
#include "pages/Pages.h"
#include <LinkedList.hpp>
#include <WifiHandler.hpp>

String maintenanceProcessor(const String &var) {
  // Firmware upload
  if (var == "pioenv_name")
    return String(PIOENV_NAME);

  return String();
}

void handleMaintenanceSetupPage(AsyncWebServerRequest *request) {
  if (!request->authenticate("admin", appcfg.admin_password)) {
    return request->requestAuthentication();
  }

  AsyncWebServerResponse *response = request->beginResponse_P(
      200, "text/html", MAINTENANCE_HTML, maintenanceProcessor);
  response->addHeader("Cache-Control", "no-cache, no-store, must-revalidate");
  response->addHeader("Pragma", "no-cache");
  response->addHeader("Expires", "0");
  request->send(response);
}
