#ifndef FSM_H_
#define FSM_H_

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef enum {
  TEMPERATURE = 0x42,
  CO2 = 0x50,
} dataType;

typedef struct {
  dataType type;
  uint16_t value;
  bool checksumIsValid;
  bool inBoot;
} mt8057_message;

// call this for each falling edge of the clock, returns true if a full frame was received
mt8057_message* mt8057_process(unsigned long ms, bool data);

#endif // FSM_H_
