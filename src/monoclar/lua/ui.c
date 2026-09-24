#include "ui.h"

void monoclar_lua_bind_ui(lua_State *L, monoclar_screen_t *screen) {
  lv_obj_t *root = monoclar_screen_get_root(screen);

  luavgl_set_root(L, root);
  luaL_requiref(L, "lvgl", luaopen_lvgl, 0);
  lua_pop(L, 1);

}

int luaopen_monoclar_screen(lua_State *L)
  {
      lv_obj_t *root = luavgl_context(L)->root;
      if (!root) {
          return luaL_error(L, "luavgl no screen root");
      }

      lua_newtable(L);

      luavgl_add_lobj(L, root)->lua_created = false;
      lua_setfield(L, -2, "root");

      return 1;
  }
