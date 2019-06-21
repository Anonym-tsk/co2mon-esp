#include "inc/include.h"

AsyncMqttClient mqttClient;
Ticker mqttReconnectTimer;

void mqttSendData(const char *topic, const char *msg) {
  if (mqttClient.connected()) {
    mqttClient.publish(topic, 0, false, msg);
    DEBUG_F("[MQTT] Message sent: %s - %s\n", topic, msg);
  }
}

void mqttSendStatus(const char *msg) {
  if (mqttClient.connected()) {
    mqttClient.publish(MQTT_TOPIC_STATUS, 0, true, msg);
    DEBUG_F("[MQTT] Status sent: %s\n", msg);
  }
}

void mqttConnect() {
  DEBUG_F("[MQTT] (Re)connecting to server on %s...\n", mqtt_server);
  ledBlinkSlow();
  mqttClient.connect();
}

void mqttStop() {
  mqttClient.disconnect();
  mqttReconnectTimer.detach();
  ledOff();
}

void mqttSetup() {
  mqttClient.setClientId(MQTT_CLIENT_ID);
  mqttClient.setCredentials(mqtt_username, mqtt_password);
  mqttClient.setWill(MQTT_TOPIC_STATUS, 0, true, MSG_STATUS_LAST_WILL);
  mqttClient.setServer(mqtt_server, atoi(mqtt_port));

  mqttClient.onConnect([](bool sessionPresent) {
    DEBUG_LN("[MQTT] Connected");
    mqttSendStatus(MSG_STATUS_READY);
    ledOff();
  });

  mqttClient.onDisconnect([](AsyncMqttClientDisconnectReason reason) {
    DEBUG_F("[MQTT] Disonnected, reason %d\n", reason);
    if (WiFi.isConnected()) {
      mqttReconnectTimer.once_ms(2000, mqttConnect);
    }
  });

  mqttConnect();
}
