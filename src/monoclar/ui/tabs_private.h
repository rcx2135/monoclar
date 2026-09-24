#pragma once

#include <lvgl.h>
#include <lvgl/lv_types.h>
#include <ui/tabs.h>

struct monoclar_tab {
  char *id;
  char *name;
  lv_obj_t *root;

  void *userdata;

  monoclar_tab_create_fn create;
  monoclar_tab_update_fn update;
  monoclar_tab_destroy_fn destroy;

  bool created;
  bool update_requested;
};
