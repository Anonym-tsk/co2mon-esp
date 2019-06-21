#include "inc/include.h"

bool _waitLow = true;
bool _waitHigh = false;
unsigned long _delayStartTime = 0;

void _mt8057ProcessMessage(mt8057_message* message) {
  char valstr[5];
  int temp;

  switch (message->type) {
    case CO2:
      if (message->value > 3000) {
        DEBUG_LN("[MT8057] Skipped high value");
      } else {
        snprintf(valstr, sizeof(valstr), "%d", message->value);
        mqttSendData(MQTT_TOPIC_MT8057_CO2, valstr);
      }
      break;
    case TEMPERATURE:
      temp = (5 * message->value - 21848) / 8;
      temp -= MT8057_CORRECTION * 10; // Коррекция из-за нагрева
      snprintf(valstr, sizeof(valstr), "%d.%d", temp / 10, abs(temp) % 10);
      mqttSendData(MQTT_TOPIC_MT8057_TEMP, valstr);
      break;
    default:
      break;
  }
}

bool mt8057IsLoopFree() {
  return _waitLow || _waitHigh;
}

void mt8057Setup() {
  pinMode(PIN_MT8057_CLOCK, INPUT);
  pinMode(PIN_MT8057_DATA, INPUT);
}

void mt8057Loop() {
  unsigned long ms = millis();

  // wait until boot
  if (_delayStartTime != 0 && ms - _delayStartTime < MT8057_BOOT_DELAY) {
    return;
  }
  _delayStartTime = 0;

  // wait until clock is high again
  if (_waitHigh) {
    if (digitalRead(PIN_MT8057_CLOCK) == LOW) {
      return;
    }
    _waitHigh = false;
  }

  // wait until clock is low
  if (_waitLow) {
    if (digitalRead(PIN_MT8057_CLOCK) != LOW) {
      return;
    }
    _waitLow = false;
  }

  bool data = (digitalRead(PIN_MT8057_DATA) == HIGH);
  mt8057_message* message = mt8057_process(ms, data);

  if (message) {
    if (!message->checksumIsValid) {
      DEBUG_LN("[MT8057] Checksum validation error");
    } else if (message->inBoot) {
      DEBUG_LN("[MT8057] Delay on start");
      _delayStartTime = ms;
    } else {
      _mt8057ProcessMessage(message);
    }
  }

  _waitLow = true;
  _waitHigh = true;
}
