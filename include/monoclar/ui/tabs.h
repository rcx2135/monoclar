#pragma once
#include <lvgl.h>
#include <stdbool.h>

typedef struct monoclar_tab monoclar_tab_t;

typedef bool (*monoclar_tab_create_fn)(monoclar_tab_t *tab, void *userdata);

typedef void (*monoclar_tab_update_fn)(monoclar_tab_t *tab, void *userdata);

typedef void (*monoclar_tab_destroy_fn)(monoclar_tab_t *tab, void *userdata);

monoclar_tab_t *monoclar_tab_create(const char *id, const char *name,
                                    monoclar_tab_create_fn create,
                                    monoclar_tab_update_fn update,
                                    monoclar_tab_destroy_fn destroy,
                                    void *userdata);

void monoclar_tab_destroy(monoclar_tab_t *tab);

lv_obj_t *monoclar_tab_get_root(const monoclar_tab_t *tab);

void monoclar_tab_request_update(monoclar_tab_t *tab);
