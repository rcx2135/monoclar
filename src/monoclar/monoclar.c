#include "lua/runtime.h"
#include "plugin/plugin_manager.h"
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

  monoclar_ctx_t *ctx = calloc(1, sizeof(monoclar_ctx_t));
  if (!ctx) {
    return NULL;
  }

  ctx->screen = monoclar_screen_create();
  if (!ctx->screen) {
    free(ctx);
    return NULL;
  }

  ctx->tabs =
      monoclar_tab_manager_create(monoclar_screen_get_content(ctx->screen));
  if (!ctx->tabs) {
    monoclar_screen_destroy(ctx->screen);
    free(ctx);
    return NULL;
  }

  ctx->plugins = monoclar_plugin_manager_create();
  if (!ctx->plugins) {
    monoclar_tab_manager_destroy(ctx->tabs);
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


  monoclar_plugin_manager_scan_directory(ctx->plugins, "plugins");

  for (size_t i = 0; i < monoclar_plugin_manager_get_count(ctx->plugins); i++) {
    monoclar_plugin_t *plugin =
        monoclar_plugin_manager_get_at(ctx->plugins, i);

    monoclar_lua_load_plugin(ctx->lua, plugin);
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
