#pragma once

#include <lvgl.h>

typedef struct monoclar_screen monoclar_screen_t;

monoclar_screen_t *monoclar_screen_create(void);
void monoclar_screen_destroy(monoclar_screen_t *screen);
lv_obj_t *monoclar_screen_get_root(const monoclar_screen_t *screen);
lv_obj_t *monoclar_screen_get_top(const monoclar_screen_t *screen);
lv_obj_t *monoclar_screen_get_content(const monoclar_screen_t *screen);
