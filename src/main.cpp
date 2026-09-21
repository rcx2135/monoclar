#include <lvgl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "drivers/display.hpp"

int main() {
  lv_init();
  auto lvgl_display = drivers::display::create_display();

  lv_group_t *g = lv_group_create();
  lv_group_set_default(g);

  while (1) {
    uint32_t ms = lv_timer_handler();
    if (ms == LV_NO_TIMER_READY) {
      ms = LV_DEF_REFR_PERIOD;
    }
    usleep(ms * 1000);
  }

  return 0;
}
