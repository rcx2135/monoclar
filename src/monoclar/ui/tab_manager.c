#include "tab_manager_private.h"
#include "tab_private.h"

#include <stdlib.h>

monoclar_tab_manager_t *monoclar_tab_manager_create(lv_obj_t *parent)
{
  monoclar_tab_manager_t *manager = calloc(1, sizeof(monoclar_tab_manager_t));
  if (!manager) {
    return NULL;
  }
  manager->parent = parent;
  return manager;
}

void monoclar_tab_manager_destroy(monoclar_tab_manager_t *manager)
{
  if (!manager) {
    return;
  }

  manager->active = NULL;
  while (manager->count) {
    monoclar_tab_t *tab = manager->tabs[--manager->count];
    tab->owner = NULL;
    monoclar_tab_destroy(tab);
  }
  free(manager->tabs);
  free(manager);
}

bool monoclar_tab_manager_register(monoclar_tab_manager_t *manager,
                                   monoclar_tab_t *tab)
{
  if (!manager) {
    return false;
  }
  if (manager->count >= manager->capacity) {
    size_t capacity = manager->capacity == 0 ? 4 : manager->capacity * 2;
    monoclar_tab_t **tabs = realloc(manager->tabs,
                                    capacity * sizeof(monoclar_tab_t *));
    if (!tabs) {
      return false;
    }
    manager->tabs = tabs;
    manager->capacity = capacity;
  }
  manager->tabs[manager->count++] = tab;
  tab->owner = manager;

  tab->root = lv_obj_create(manager->parent);
  if (!tab->root) {
    return false;
  }

  lv_obj_remove_style_all(tab->root);
  lv_obj_set_size(tab->root, LV_PCT(100), LV_PCT(100));
  lv_obj_set_pos(tab->root, 0, 0);
  lv_obj_set_scrollable(tab->root, false);
  lv_obj_set_hidden(tab->root, true);

  return true;
}

bool monoclar_tab_manager_unregister(monoclar_tab_manager_t *manager,
                                     monoclar_tab_t *tab)
{
  return false;
}

bool monoclar_tab_manager_set_active(monoclar_tab_manager_t *manager,
                                       monoclar_tab_t *tab)
  {
    if (!manager) {
      return false;
    }

    if (tab && (tab->owner != manager || !tab->root)) {
      return false;
    }

    if (manager->active == tab) {
      return true;
    }

    if (manager->active && manager->active->root) {
      lv_obj_set_hidden(manager->active->root, true);
    }

    manager->active = tab;

    if (tab) {
      lv_obj_set_hidden(tab->root, false);
    }

    return true;
  }

monoclar_tab_t *monoclar_tab_manager_get_active(
    const monoclar_tab_manager_t *manager)
{
  return manager->active;
}
