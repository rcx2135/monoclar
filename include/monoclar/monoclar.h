#pragma once

#include <stdbool.h>
#include <ui/tab_manager.h>

typedef struct monoclar_ctx monoclar_ctx_t;

monoclar_ctx_t *monoclar_create(void);
void monoclar_run(monoclar_ctx_t *ctx);
void monoclar_destroy(monoclar_ctx_t *ctx);

monoclar_tab_manager_t *monoclar_get_tab_manager(const monoclar_ctx_t *ctx);
