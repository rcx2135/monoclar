#pragma once

#include <lvgl.h>
#include <ui/screen.h>

struct monoclar_screen {
  lv_display_t *display;
  lv_obj_t *root;
  lv_obj_t *top;
  lv_obj_t *content;
};
