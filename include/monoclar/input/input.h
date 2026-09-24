#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct monoclar_input monoclar_input_t;
typedef struct monoclar_input_subscription monoclar_input_subscription_t;

typedef enum {
  MONOCLAR_INPUT_TOUCH_BEGIN,
  MONOCLAR_INPUT_TOUCH_MOVE,
  MONOCLAR_INPUT_TOUCH_END,
  MONOCLAR_INPUT_TOUCH_CANCEL
} monoclar_input_event_type_t;

typedef struct {
  monoclar_input_event_type_t type;
  uint64_t time_us;
  float x;
  float y;
} monoclar_input_event_t;

typedef void (*monoclar_input_event_fn)(const monoclar_input_event_t *event,
                                       void *userdata);

monoclar_input_subscription_t *monoclar_input_subscribe(
    monoclar_input_t *input, monoclar_input_event_type_t type,
    monoclar_input_event_fn callback, void *userdata);

void monoclar_input_unsubscribe(monoclar_input_subscription_t *subscription);

#ifdef __cplusplus
}
#endif
