#include "tab_manager_private.h"
#include "tabs_private.h"

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
  manager->active = tab;
  return true;
}

monoclar_tab_t *monoclar_tab_manager_get_active(
    const monoclar_tab_manager_t *manager)
{
  return manager->active;
}
