#include "monoclar.h"
#include <stdio.h>

int main() {

  monoclar_ctx_t *ctx = monoclar_create();
  if (!ctx) {
    fprintf(stderr, "Failed to create monoclar context\n");
    return 1;
  }

  monoclar_run(ctx);
  monoclar_destroy(ctx);

  return 0;
}
