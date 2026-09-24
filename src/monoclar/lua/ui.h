#pragma once

#include <lauxlib.h>
#include <lua.h>
#include <lualib.h>
#include <luavgl.h>
#include <ui/screen.h>

void monoclar_lua_bind_ui(lua_State *L, monoclar_screen_t *screen);
int luaopen_monoclar_screen(lua_State *L);
