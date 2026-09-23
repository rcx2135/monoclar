#include "monoclar.h"
#include <stdio.h>

int main() {

  monoclar_ctx_t *ctx = monoclar_create();
  if (!ctx) {
    fprintf(stderr, "Failed to create monoclar context\n");
    return 1;
  }

  monoclar_run(ctx);

  return 0;
}

// lv_init();
// lv_display_t *display = display_create();
// if (!display) {
//   fprintf(stderr, "Failed to create display\n");
//   return 1;
// }

// lv_group_t *g = lv_group_create();
// lv_group_set_default(g);

// lua_State *L = luaL_newstate();
// luaL_openlibs(L);
// const char *code = "print('Hello from a string!')";

// if (luaL_dostring(L, code) != LUA_OK) {
//   const char *message = lua_tostring(L, -1);
//   fprintf(stderr, "Lua: %s\n", message ? message : "Unknown error");
//   lua_pop(L, 1);
// }

// while (1) {
//   uint32_t ms = lv_timer_handler();
//   if (ms == LV_NO_TIMER_READY) {
//     ms = LV_DEF_REFR_PERIOD;
//   }
//   usleep(ms * 1000);
// }
