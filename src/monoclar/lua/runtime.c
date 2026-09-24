#include "runtime.h"
#include "ui.h"

#include <lauxlib.h>
#include <lualib.h>
#include <luavgl.h>




lua_State *monoclar_lua_create(monoclar_tab_manager_t *tabs,
                              monoclar_screen_t *screen) {
  lua_State *L = luaL_newstate();
  if (!L) {
    return NULL;
  }

  luaL_openlibs(L);
  monoclar_lua_bind_ui(L, screen);
  monoclar_lua_bind_tabs(L, tabs);

  luaL_requiref(L, "monoclar.screen", luaopen_monoclar_screen, 0);
  lua_pop(L, 1);


  return L;
}

void monoclar_lua_destroy(lua_State *L)
{

}
