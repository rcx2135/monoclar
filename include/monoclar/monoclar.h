#pragma once

typedef struct monoclar_ctx monoclar_ctx_t;

monoclar_ctx_t *monoclar_create(void);
void monoclar_run(monoclar_ctx_t *ctx);
void monoclar_destroy(monoclar_ctx_t *ctx);
