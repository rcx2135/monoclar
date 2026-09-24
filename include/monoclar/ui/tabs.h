#pragma once
#include <lvgl.h>
#include <stdbool.h>

typedef struct monoclar_tab monoclar_tab_t;

monoclar_tab_t *monoclar_tab_create(const char *id, const char *name);

void monoclar_tab_destroy(monoclar_tab_t *tab);

lv_obj_t *monoclar_tab_get_root(const monoclar_tab_t *tab);
