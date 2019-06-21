#include "inc/include.h"

bool _shouldSaveConfig = false;

void wifiSetup() {
  ledBlinkFast();
  readConfig();

  char info_str[160];
  sprintf(info_str, "<p>%s<br/>%s<br/>%s</p>", MQTT_TOPIC_STATUS, MQTT_TOPIC_MT8057_TEMP, MQTT_TOPIC_MT8057_CO2);

  WiFiManagerParameter param_mqtt_server("server", "MQTT Server", mqtt_server, 40, "type='text' required");
  WiFiManagerParameter param_mqtt_port("port", "MQTT Port", mqtt_port, 6, "type='number' required");
  WiFiManagerParameter param_mqtt_username("username", "MQTT Username", mqtt_username, 40, "type='text' required");
  WiFiManagerParameter param_mqtt_password("password", "MQTT Password", mqtt_password, 40, "type='password' required");
  WiFiManagerParameter info_text(info_str);

  char ssid[80];
  sprintf(ssid, "%s_%i", WIFI_AP_SSID, ESP.getChipId());

  WiFiManager wifiManager;
  wifiManager.setDebugOutput(false);
  wifiManager.setConfigPortalTimeout(60);
  wifiManager.setConnectTimeout(60);
  wifiManager.setSaveConfigCallback([]() {
    _shouldSaveConfig = true;
  });
  wifiManager.setAPCallback([](WiFiManager *myWiFiManager) {
    DEBUG_LN("[WIFI] Entered AP mode");
  });

  wifiManager.addParameter(&param_mqtt_server);
  wifiManager.addParameter(&param_mqtt_port);
  wifiManager.addParameter(&param_mqtt_username);
  wifiManager.addParameter(&param_mqtt_password);
  wifiManager.addParameter(&info_text);

  DEBUG_LN("[WIFI] Connecting to wifi");
  if (!wifiManager.autoConnect(ssid) || !WiFi.isConnected()) {
    DEBUG_LN("[WIFI] Failed to connect and hit timeout");
    delay(3000);
    ESP.reset();
  }

  DEBUG_LN("[WIFI] Connected");
  DEBUG_F("[WIFI] IP address: %s\n", WiFi.localIP().toString().c_str());

  strcpy(mqtt_server, param_mqtt_server.getValue());
  strcpy(mqtt_port, param_mqtt_port.getValue());
  strcpy(mqtt_username, param_mqtt_username.getValue());
  strcpy(mqtt_password, param_mqtt_password.getValue());

  if (strlen(mqtt_server) == 0 || strlen(mqtt_port) == 0 ||
      strlen(mqtt_username) == 0 || strlen(mqtt_password) == 0) {
    DEBUG_LN("[WIFI] Broken configuration, reset settings");
    wifiManager.resetSettings();
    delay(3000);
    ESP.reset();
  }

  if (_shouldSaveConfig) {
    saveConfig();
  }

  ledOff();
}
