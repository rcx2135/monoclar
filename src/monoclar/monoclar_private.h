#pragma once
#include <lua.h>
#include <lvgl.h>
#include <monoclar.h>
#include <stdbool.h>
#include <ui/screen.h>

struct monoclar_ctx {
  monoclar_screen_t *screen;
  lua_State *lua;
  bool running;
};
