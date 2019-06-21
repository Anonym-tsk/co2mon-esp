#include "mt8057_fsm.h"

// max time between bits, until a new frame is assumed to have started
#define MT8057_MAX_MS  2
#define BITS_IN_BYTE   8
#define FRAME_SIZE     40

#define BYTE_TYPE  0
#define BYTE_HIGH  1
#define BYTE_LOW   2
#define BYTE_SUM   3
#define BYTE_END   4

#define DELIMETER  0x0D

static uint8_t buffer[5];
static int num_bits = 0;
static unsigned long prev_ms;

static mt8057_message _msg;
static mt8057_message *msg = &_msg;

// Декодирует сообщение
void mt8057_decode(void) {
  uint8_t checksum = buffer[BYTE_TYPE] + buffer[BYTE_HIGH] + buffer[BYTE_LOW];
  msg->checksumIsValid = (checksum == buffer[BYTE_SUM] && buffer[BYTE_END] == DELIMETER);
  if (!msg->checksumIsValid) {
    return;
  }

  msg->type = (dataType)buffer[BYTE_TYPE];
  // Получение значения показателя
  msg->value = buffer[BYTE_HIGH] << BITS_IN_BYTE | buffer[BYTE_LOW];
  // Еще загружаемся
  msg->inBoot = (msg->type == CO2 && msg->value > 10000);
}

mt8057_message* mt8057_process(unsigned long ms, bool data) {
  // check if a new message has started, based on time since previous bit
  if ((ms - prev_ms) > MT8057_MAX_MS) {
    num_bits = 0;
  }
  prev_ms = ms;

  // number of bits received is basically the "state"
  if (num_bits < FRAME_SIZE) {
    // store it while it fits
    int idx = num_bits / BITS_IN_BYTE;
    buffer[idx] = (buffer[idx] << 1) | (data ? 1 : 0);
    // are we done yet?
    num_bits++;
    if (num_bits == FRAME_SIZE) {
      mt8057_decode();
      return msg;
    }
  }

  return nullptr;
}
