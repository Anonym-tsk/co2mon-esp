#ifndef SW_H_
#define SW_H_

// Software configuration
#define HOST_NAME                 "co2mon"
#define HOST_PASSWORD             "co2mon"
#define OTA_PORT                  8266

#define WIFI_AP_SSID              "CO2Mon"

#define CONFIG_FILE               "/config.json"

#define MT8057_BOOT_DELAY         20000 // reading data delay after power on
#define MT8057_CORRECTION         0 // temperature correction (sensor_temp minus MT8057_CORRECTION)

#define MSG_STATUS_READY          "R" // ready; sent after successfull boot-up
#define MSG_STATUS_LAST_WILL      "L" // last will message; send when device goes offline

#define MQTT_CLIENT_ID            "co2mon"

#define MQTT_TOPIC_STATUS         "co2mon/status"
#define MQTT_TOPIC_MT8057_TEMP    "co2mon/temp"
#define MQTT_TOPIC_MT8057_CO2     "co2mon/co2"

#endif // SW_H_
