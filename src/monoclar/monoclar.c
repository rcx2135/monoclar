#include <lauxlib.h>
#include <lualib.h>
#include <monoclar.h>
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

  ctx->lua = luaL_newstate();
  if (!ctx->lua) {
    // destroy screen make later
    free(ctx);
    return NULL;
  }
  luaL_openlibs(ctx->lua);

  ctx->running = true;

  return ctx;
}


void monoclar_run(monoclar_ctx_t *ctx) {
  while (ctx->running) {
    uint32_t ms = lv_timer_handler();
    if (ms == LV_NO_TIMER_READY) {
      ms = LV_DEF_REFR_PERIOD;
    }
    usleep(ms * 1000);
  }
}
