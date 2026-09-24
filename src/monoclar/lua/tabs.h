#pragma once

#include <lua.h>
#include <ui/tab_manager.h>

void monoclar_lua_bind_tabs(lua_State *L, monoclar_tab_manager_t *manager);
void monoclar_lua_destroy_tabs(lua_State *L);
