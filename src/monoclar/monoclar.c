#include "lua/runtime.h"
#include <lauxlib.h>
#include <lua.h>
#include <monoclar.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "monoclar_private.h"

monoclar_ctx_t *monoclar_create(void) {
  lv_init();

  monoclar_ctx_t *ctx = malloc(sizeof(monoclar_ctx_t));
  if (!ctx) {
    return NULL;
  }

  ctx->screen = monoclar_screen_create();
  if (!ctx->screen) {
    free(ctx);
    return NULL;
  }

  ctx->lua = monoclar_lua_create(ctx->screen);
  if (!ctx->lua) {
    // destroy screen make later
    free(ctx);
    return NULL;
  }
  ctx->running = true;

  return ctx;
}

void monoclar_run(monoclar_ctx_t *ctx) {

  // test
  //
  //

  const char *code = " local lvgl = require(\"lvgl\")\n\n  monoclar.screen.root:Label {\n      text = \"Hello from Lua\",\n      align = lvgl.ALIGN.CENTER,\n  }";
  if (luaL_dostring(ctx->lua, code) != LUA_OK) {
    const char *message = lua_tostring(ctx->lua, -1);
    fprintf(stderr, "Lua: %s\n", message ? message : "Unknown error");
    lua_pop(ctx->lua, 1);
  }

  while (ctx->running) {
    uint32_t ms = lv_timer_handler();
    if (ms == LV_NO_TIMER_READY) {
      ms = LV_DEF_REFR_PERIOD;
    }
    usleep(ms * 1000);
  }
}
