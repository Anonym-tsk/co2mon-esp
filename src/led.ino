#include "inc/include.h"

Ticker ledBlinker;

void _blink() {
  digitalWrite(PIN_LED, !digitalRead(PIN_LED));
}

void _ledBlink(int speedMs) {
  ledBlinker.attach_ms(speedMs, _blink);
}

void ledSetup() {
  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LED_OFF);
}

void ledBlinkSlow() {
  ledOff();
  _ledBlink(800);
}

void ledBlinkFast() {
  ledOff();
  _ledBlink(300);
}

void ledOff() {
  ledBlinker.detach();
  digitalWrite(PIN_LED, LED_OFF);
}

void ledOn() {
  digitalWrite(PIN_LED, LED_ON);
}
