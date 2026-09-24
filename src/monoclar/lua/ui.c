#include "ui.h"

static const char screen_key;

void monoclar_lua_bind_ui(lua_State *L, monoclar_screen_t *screen) {
  lv_obj_t *root = monoclar_screen_get_root(screen);

  luavgl_set_root(L, root);
  luaL_requiref(L, "lvgl", luaopen_lvgl, 0);
  lua_pop(L, 1);

  lv_obj_set_style_bg_color(root, lv_color_black(), 0);
  lv_obj_set_style_bg_opa(root, LV_OPA_COVER, 0);

  lua_pushlightuserdata(L, screen);
  lua_rawsetp(L, LUA_REGISTRYINDEX, &screen_key);

}

int luaopen_monoclar_screen(lua_State *L)
{
  lua_rawgetp(L, LUA_REGISTRYINDEX, &screen_key);
  monoclar_screen_t *screen = lua_touserdata(L, -1);
  lua_pop(L, 1);
  if (!screen || !luavgl_context(L)->root) {
    return luaL_error(L, "luavgl has no screen root");
  }

  lua_newtable(L);
  luavgl_add_lobj(L, monoclar_screen_get_root(screen))->lua_created = false;
  lua_setfield(L, -2, "root");
  luavgl_add_lobj(L, monoclar_screen_get_top(screen))->lua_created = false;
  lua_setfield(L, -2, "top");
  luavgl_add_lobj(L, monoclar_screen_get_content(screen))->lua_created = false;
  lua_setfield(L, -2, "content");
  return 1;
}
