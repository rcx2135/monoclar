#include "runtime.h"

#include <lauxlib.h>
#include <lualib.h>
#include <luavgl.h>

static void register_monoclar(lua_State *L, lv_obj_t *root) {
  lua_newtable(L);
  lua_newtable(L);

  luavgl_add_lobj(L, root)->lua_created = false;
  lua_setfield(L, -2, "root");

  lua_setfield(L, -2, "screen");
  lua_setglobal(L, "monoclar");
}

lua_State *monoclar_lua_create(monoclar_screen_t *screen) {
  lua_State *L = luaL_newstate();
  if (!L) {
    return NULL;
  }

  luaL_openlibs(L);

  lv_obj_t *root = monoclar_screen_get_root(screen);

  luavgl_set_root(L, root);
  luaL_requiref(L, "lvgl", luaopen_lvgl, 0);
  lua_pop(L, 1);

  lv_obj_set_size(root, lv_pct(100), lv_pct(100));

  register_monoclar(L, root);

  return L;
}
