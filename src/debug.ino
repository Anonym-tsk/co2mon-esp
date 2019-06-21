#include "inc/include.h"

WiFiServer server(23);
WiFiClient client;

void debugSetup() {
  Serial.begin(115200);
  server.begin();
  server.setNoDelay(true);
}

void debugLoop() {
  if (server.hasClient() && !client.connected()) {
    client.stop();
    client = server.available();
  }
}

void DEBUG_LN(const char *text) {
  Serial.println(text);
  if (client.connected()) {
    unsigned int len = strlen(text) + 2;
    char data[len];
    sprintf(data, "%s\n", text);
    client.write(data, len);
  }
}

template<typename... Args>
void DEBUG_F(const char *format, Args... args) {
  Serial.printf(format, args...);
  if (client.connected()) {
    char data[128];
    sprintf(data, format, args...);
    client.write(data, strlen(data));
  }
}
