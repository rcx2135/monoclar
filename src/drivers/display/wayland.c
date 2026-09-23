#include "../display.h"


lv_display_t *display_create() {
  return lv_wayland_window_create(500, 500, "", NULL);
}
