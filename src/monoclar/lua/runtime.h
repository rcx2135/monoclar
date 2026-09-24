#pragma once

#include "tabs.h"
#include <lua.h>
#include <plugin/plugin.h>
#include <stdbool.h>
#include <ui/screen.h>
#include <ui/tab_manager.h>

lua_State *monoclar_lua_create(monoclar_tab_manager_t *tabs,
                               monoclar_screen_t *screen);

bool monoclar_lua_load_plugin(lua_State *L, const monoclar_plugin_t *plugin);
