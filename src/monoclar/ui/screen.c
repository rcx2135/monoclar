#include "../../drivers/display.h"
#include "screen_private.h"
#include <stdlib.h>

static void screen_layout(lv_event_t *event)
{
  monoclar_screen_t *screen = lv_event_get_user_data(event);
  int32_t height = lv_obj_get_height(screen->root);
  int32_t top_height = lv_obj_get_height(screen->top);
  lv_obj_set_pos(screen->content, 0, top_height);
  lv_obj_set_size(screen->content, lv_obj_get_width(screen->root),
                  height > top_height ? height - top_height : 0);
}

monoclar_screen_t *monoclar_screen_create(void) {
  monoclar_screen_t *screen = calloc(1, sizeof(monoclar_screen_t));
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

  screen->top = lv_obj_create(screen->root);
  screen->content = lv_obj_create(screen->root);
  if (!screen->top || !screen->content) {
    monoclar_screen_destroy(screen);
    return NULL;
  }

  lv_obj_remove_style_all(screen->top);
  lv_obj_set_size(screen->top, LV_PCT(100), LV_SIZE_CONTENT);
  lv_obj_set_flex_flow(screen->top, LV_FLEX_FLOW_ROW);
  lv_obj_set_scrollable(screen->top, false);
  lv_obj_remove_style_all(screen->content);
  lv_obj_set_size(screen->content, LV_PCT(100), LV_PCT(100));
  lv_obj_set_scrollable(screen->content, false);

  lv_obj_add_event_cb(screen->root, screen_layout, LV_EVENT_SIZE_CHANGED, screen);
  lv_obj_add_event_cb(screen->top, screen_layout, LV_EVENT_SIZE_CHANGED, screen);
  lv_obj_update_layout(screen->root);

  return screen;
}

void monoclar_screen_destroy(monoclar_screen_t *screen)
{
  if (!screen) {
    return;
  }
  if (screen->root) {
    lv_obj_remove_event_cb(screen->root, screen_layout);
  }
  if (screen->top) {
    lv_obj_remove_event_cb(screen->top, screen_layout);
  }
  lv_display_delete(screen->display);
  free(screen);
}

lv_obj_t *monoclar_screen_get_root(const monoclar_screen_t *screen) {
  return screen->root;
}

lv_obj_t *monoclar_screen_get_top(const monoclar_screen_t *screen)
{
  return screen->top;
}

lv_obj_t *monoclar_screen_get_content(const monoclar_screen_t *screen)
{
  return screen->content;
}
