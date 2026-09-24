#pragma once

#include <lvgl.h>
#include <lvgl/lv_types.h>
#include <ui/tab_manager.h>
#include <ui/tabs.h>

struct monoclar_tab {
  char *id;
  char *name;
  lv_obj_t *root;
  monoclar_tab_manager_t *owner;

  void *userdata;

  monoclar_tab_create_fn create;
  monoclar_tab_update_fn update;
  monoclar_tab_destroy_fn destroy;

  bool created;
  bool update_requested;
};
