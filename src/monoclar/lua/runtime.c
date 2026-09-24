#include "runtime.h"
#include "../plugin/plugin_private.h"
#include "ui.h"

#include <lauxlib.h>
#include <lualib.h>
#include <luavgl.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int callback_traceback(lua_State *L) {
  const char *message = lua_tostring(L, 1);
  if (!message) {
    message = lua_pushfstring(L, "error object is a %s value",
                             luaL_typename(L, 1));
  }

  luaL_traceback(L, L, message, 1);
  return 1;
}

static int callback_pcall(lua_State *L, int nargs, int nresults) {
  int base = lua_gettop(L) - nargs;
  lua_pushcfunction(L, callback_traceback);
  lua_insert(L, base);

  int status = lua_pcall(L, nargs, nresults, base);
  if (status != LUA_OK) {
    const char *message = lua_tostring(L, -1);
    fprintf(stderr, "Lua callback: %s\n",
            message ? message : "non-string Lua error");
  }

  lua_remove(L, base);
  return status;
}

lua_State *monoclar_lua_create(monoclar_tab_manager_t *tabs,
                              monoclar_screen_t *screen) {
  lua_State *L = luaL_newstate();
  if (!L) {
    return NULL;
  }

  luaL_openlibs(L);
  luavgl_set_pcall(L, callback_pcall);
  monoclar_lua_bind_ui(L, screen);
  monoclar_lua_bind_tabs(L, tabs);

  luaL_requiref(L, "monoclar.screen", luaopen_monoclar_screen, 0);
  lua_pop(L, 1);


  return L;
}

bool monoclar_lua_load_plugin(lua_State *L, const monoclar_plugin_t *plugin) {
  if (!L || !plugin || plugin->type != MONOCLAR_PLUGIN_LUA) {
    return false;
  }

  size_t length = strlen(plugin->directory);
  if (length > SIZE_MAX - sizeof("/main.lua")) {
    return false;
  }

  size_t size = length + sizeof("/main.lua");
  char *path = malloc(size);
  if (!path) {
    fprintf(stderr, "Lua plugin '%s': cannot allocate entry path\n", plugin->id);
    return false;
  }

  snprintf(path, size, "%s/main.lua", plugin->directory);

  int top = lua_gettop(L);
  int status = luaL_loadfile(L, path);
  free(path);

  if (status == LUA_OK) {
    status = lua_pcall(L, 0, 0, 0);
  }

  if (status != LUA_OK) {
    const char *message = lua_tostring(L, -1);
    fprintf(stderr, "Lua plugin '%s': %s\n", plugin->id,
            message ? message : "non-string Lua error");
  }

  lua_settop(L, top);
  return status == LUA_OK;
}

void monoclar_lua_destroy(lua_State *L)
{

}
