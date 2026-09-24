
#pragma once

#include <stdint.h>

typedef struct input input_t;

typedef enum {
  INPUT_BEGIN,
  INPUT_MOVE,
  INPUT_END,
  INPUT_CANCEL
} input_event_type_t;

typedef struct driver_input_event {
  input_event_type_t type;
  uint64_t time_us;
  float dx;
  float dy;
} input_event_t;

typedef void (*input_event_fn)(const input_event_t *event, void *userdata);

input_t *input_create(const char *path, input_event_fn callback,
                      void *userdata);
int input_poll(input_t *input);
void input_destroy(input_t *input);
