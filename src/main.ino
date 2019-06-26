#include "inc/include.h"

void setup() {
  debugSetup();
  DEBUG_LN("\nMT8057 CO2 Sensor\n");

  // ESP.eraseConfig();
  // SPIFFS.format();

  ledSetup();
  wifiSetup();
  otaSetup();
  mqttSetup();
  mt8057Setup();
}

void loop() {
  if (mt8057IsLoopFree()) {
    otaLoop();
    debugLoop();
  }
  mt8057Loop();
}
