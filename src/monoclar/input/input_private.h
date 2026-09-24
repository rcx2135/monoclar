#pragma once

#include "../../drivers/input.h"
#include <input/input.h>

monoclar_input_t *monoclar_input_create(void);
void monoclar_input_destroy(monoclar_input_t *input);

void monoclar_input_process(monoclar_input_t *input, const input_event_t *event);
