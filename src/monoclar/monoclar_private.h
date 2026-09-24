#pragma once
#include <lua.h>
#include <lvgl.h>
#include <monoclar.h>
#include <stdbool.h>
#include <ui/screen.h>
#include <ui/tabs.h>

struct monoclar_ctx {
  monoclar_screen_t *screen;
  lua_State *lua;
  monoclar_tab_t **tabs;
  size_t tab_count;
  size_t tab_capacity;
  monoclar_tab_t *active_tab;
  bool running;
};
