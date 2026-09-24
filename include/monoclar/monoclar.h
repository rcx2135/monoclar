#pragma once

#include <stdbool.h>
#include <ui/tabs.h>

typedef struct monoclar_ctx monoclar_ctx_t;

monoclar_ctx_t *monoclar_create(void);
void monoclar_run(monoclar_ctx_t *ctx);
void monoclar_destroy(monoclar_ctx_t *ctx);

bool monoclar_register_tab(monoclar_ctx_t *ctx, monoclar_tab_t *tab);

bool monoclar_unregister_tab(monoclar_ctx_t *ctx, monoclar_tab_t *tab);

bool monoclar_set_active_tab(monoclar_ctx_t *ctx, monoclar_tab_t *tab);

monoclar_tab_t *monoclar_get_active_tab(const monoclar_ctx_t *ctx);
