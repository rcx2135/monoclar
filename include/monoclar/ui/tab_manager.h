#pragma once

#include <ui/tabs.h>

typedef struct monoclar_tab_manager monoclar_tab_manager_t;

monoclar_tab_manager_t *monoclar_tab_manager_create(lv_obj_t *parent);
void monoclar_tab_manager_destroy(monoclar_tab_manager_t *manager);

bool monoclar_tab_manager_register(monoclar_tab_manager_t *manager,
                                   monoclar_tab_t *tab);
bool monoclar_tab_manager_unregister(monoclar_tab_manager_t *manager,
                                     monoclar_tab_t *tab);
bool monoclar_tab_manager_set_active(monoclar_tab_manager_t *manager,
                                     monoclar_tab_t *tab);
monoclar_tab_t *monoclar_tab_manager_get_active(
    const monoclar_tab_manager_t *manager);
