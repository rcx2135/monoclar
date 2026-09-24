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

  lv_obj_t *parent = lv_display_get_screen_active(screen->display);

  screen->root = lv_obj_create(parent);

  if (!screen->root) {
    lv_display_delete(screen->display);
    free(screen);
    return NULL;
  }
  lv_obj_remove_style_all(screen->root);
  lv_obj_set_style_bg_color(screen->root, lv_color_black(), 0);

  lv_obj_set_style_bg_opa(screen->root, LV_OPA_COVER, 0);
  lv_obj_set_size(screen->root, LV_PCT(100), LV_PCT(100));
  lv_obj_set_pos(screen->root, 0, 0);

  return screen;
}

lv_obj_t *monoclar_screen_get_root(const monoclar_screen_t *screen) {
  return screen->root;
}
