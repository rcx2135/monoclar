#pragma once

#include "tabs.h"
#include <lua.h>
#include <ui/tab_manager.h>
#include <ui/screen.h>

lua_State *monoclar_lua_create(monoclar_tab_manager_t *tabs,
                              monoclar_screen_t *screen);
