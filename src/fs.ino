#include "inc/include.h"

char mqtt_server[40];
char mqtt_port[6] = "1883";
char mqtt_username[40];
char mqtt_password[40];

void readConfig() {
  DEBUG_LN("[FS] Try to read config file");

  if (!SPIFFS.begin()) {
    DEBUG_LN("[FS] Failed to mount FS");
    return;
  }

  DEBUG_LN("[FS] Mounted file system");
  if (!SPIFFS.exists(CONFIG_FILE)) {
    DEBUG_LN("[FS] Config file not found");
    SPIFFS.end();
    return;
  }

  DEBUG_LN("[FS] Reading config file");
  File configFile = SPIFFS.open(CONFIG_FILE, "r");
  if (!configFile) {
    DEBUG_LN("[FS] Failed to load json config");
    SPIFFS.end();
    return;
  }

  DEBUG_LN("[FS] Opened config file");
  size_t size = configFile.size();
  std::unique_ptr<char[]> buf(new char[size]);
  configFile.readBytes(buf.get(), size);

  DynamicJsonDocument json(1024);
  DeserializationError error = deserializeJson(json, buf.get());
  if (error) {
    DEBUG_LN("[FS] Failed to load json config");
    configFile.close();
    SPIFFS.end();
    return;
  }

  strcpy(mqtt_server, json["mqtt_server"]);
  strcpy(mqtt_port, json["mqtt_port"]);
  strcpy(mqtt_username, json["mqtt_username"]);
  strcpy(mqtt_password, json["mqtt_password"]);

  configFile.close();
  SPIFFS.end();

  DEBUG_F("[FS] Config parsed - %s:*****@%s:%s\n", mqtt_username, mqtt_server, mqtt_port);
}

void saveConfig() {
  DEBUG_LN("[FS] Saving config");

  if (!SPIFFS.begin()) {
    DEBUG_LN("[FS] Failed to mount FS");
    return;
  }

  DynamicJsonDocument json(1024);
  json["mqtt_server"] = mqtt_server;
  json["mqtt_port"] = mqtt_port;
  json["mqtt_username"] = mqtt_username;
  json["mqtt_password"] = mqtt_password;

  File configFile = SPIFFS.open(CONFIG_FILE, "w");
  if (!configFile) {
    DEBUG_LN("[FS] Failed to open config file for writing");
    SPIFFS.end();
    return;
  }

  serializeJson(json, configFile);
  configFile.close();
  SPIFFS.end();

  DEBUG_LN("[FS] Config saved");
}
