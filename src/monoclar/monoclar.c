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

  ctx->lua = monoclar_lua_create(ctx->screen);

  if (!ctx->lua) {
    // destroy screen make later
    free(ctx);
    return NULL;
  }

  return ctx;
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


bool monoclar_register_tab(monoclar_ctx_t *ctx, monoclar_tab_t *tab) {
    if (ctx->tab_count >= ctx->tab_capacity) {
        size_t new_capacity = ctx->tab_capacity == 0 ? 4 : ctx->tab_capacity * 2;
        monoclar_tab_t **new_tabs = realloc(ctx->tabs, new_capacity * sizeof(monoclar_tab_t *));
        if (!new_tabs) {
            return false;
        }
        ctx->tabs = new_tabs;
        ctx->tab_capacity = new_capacity;
    }

    ctx->tabs[ctx->tab_count++] = tab;

    return true;
}
bool monoclar_unregister_tab(monoclar_ctx_t *ctx, monoclar_tab_t *tab) {
return false;
}

bool monoclar_set_active_tab(monoclar_ctx_t *ctx, monoclar_tab_t *tab) {
    ctx->active_tab = tab;
    return true;
}

monoclar_tab_t *monoclar_get_active_tab(const monoclar_ctx_t *ctx) {
    return ctx->active_tab;
}
