#include <Arduino.h>
#include <FS.h>
#include <Ticker.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <ArduinoOTA.h>
#include <AsyncMqttClient.h>
#include <ArduinoJson.h>

#include "../config/hardware.h"
#include "../config/software.h"
#include "../legacy/mt8057_fsm.h"
