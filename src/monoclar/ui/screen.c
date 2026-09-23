#include "../../drivers/display.h"
#include "screen_private.h"
#include <stdlib.h>

monoclar_screen_t *monoclar_screen_create(void) {
  monoclar_screen_t *screen = malloc(sizeof(monoclar_screen_t));
  if (!screen) {
    return NULL;
  }

  screen->display = display_create();
  if (!screen->display) {
    free(screen);
    return NULL;
  }

  screen->root = lv_obj_create(lv_display_get_screen_active(screen->display));
  if (!screen->root) {
    lv_display_delete(screen->display);
    free(screen);
    return NULL;
  }

  return screen;
}


lv_obj_t *monoclar_screen_get_root(const monoclar_screen_t *screen)
{
    return screen->root;
}
