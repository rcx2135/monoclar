#pragma once
#include <lua.h>
#include <lvgl.h>
#include <monoclar.h>
#include <stdbool.h>
#include <ui/screen.h>
#include <ui/tab_manager.h>

struct monoclar_ctx {
  monoclar_screen_t *screen;
  lua_State *lua;
  monoclar_tab_manager_t *tabs;
  bool running;
};
