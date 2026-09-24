#include "lua/runtime.h"
#include "ui/screen.h"
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

  ctx->tabs = monoclar_tab_manager_create(monoclar_screen_get_content(ctx->screen));
  if (!ctx->tabs) {
    monoclar_screen_destroy(ctx->screen);
    free(ctx);
    return NULL;
  }

  ctx->lua = monoclar_lua_create(ctx->tabs, ctx->screen);

  if (!ctx->lua) {
    monoclar_tab_manager_destroy(ctx->tabs);
    monoclar_screen_destroy(ctx->screen);
    free(ctx);
    return NULL;
  }

  return ctx;
}

void monoclar_destroy(monoclar_ctx_t *ctx) {
  if (!ctx) {
    return;
  }

  ctx->running = false;

  // monoclar_lua_destroy(ctx->lua);
  monoclar_tab_manager_destroy(ctx->tabs);
  monoclar_screen_destroy(ctx->screen);
  free(ctx);
}

void monoclar_run(monoclar_ctx_t *ctx) {
  ctx->running = true;

  // test
  //
  //

  const char *code =
      "local lvgl = require(\"lvgl\")"
      "local screen = require(\"monoclar.screen\")"
      "local font = "
      "lvgl.FontFromFile(\"/home/nophono/coding/monoclar/tools/fonts/"
      "Space_Mono/SpaceMono-Regular.ttf\", 27)"
      "screen.root:Label({text = \"Hello from Lua\", text_font=font, align = "
      "lvgl.ALIGN.CENTER})";

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


monoclar_tab_manager_t *monoclar_get_tab_manager(const monoclar_ctx_t *ctx) {
    return ctx->tabs;
}
