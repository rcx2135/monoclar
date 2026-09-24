#pragma once

#include <stddef.h>
#include <ui/tab_manager.h>

struct monoclar_tab_manager {
  monoclar_tab_t **tabs;
  size_t count;
  size_t capacity;
  monoclar_tab_t *active;
  lv_obj_t *parent;
};
