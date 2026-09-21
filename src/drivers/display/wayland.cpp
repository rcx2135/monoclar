#include "../display.hpp"

namespace drivers::display {
lv_display_t *create_display() {
  return lv_wayland_window_create(500, 500, "", NULL);
}
} // namespace drivers::display
