#include "tab_private.h"

#include <stdlib.h>
#include <string.h>

monoclar_tab_t *monoclar_tab_create(const char *id, const char *name)
{
  if (!id || !*id || !name) {
    return NULL;
  }

  monoclar_tab_t *tab = calloc(1, sizeof(monoclar_tab_t));
  if (!tab) {
    return NULL;
  }

  *tab = (monoclar_tab_t){0};
  tab->id = strdup(id);
  tab->name = strdup(name);
  if (!tab->id || !tab->name) {
    free(tab->id);
    free(tab->name);
    free(tab);
    return NULL;
  }
  return tab;
}

void monoclar_tab_destroy(monoclar_tab_t *tab)
{
  if (!tab || tab->owner) {
    return;
  }
  if (tab->root) {
    lv_obj_delete(tab->root);
  }
  free(tab->id);
  free(tab->name);
  free(tab);
}

lv_obj_t *monoclar_tab_get_root(const monoclar_tab_t *tab)
{
  return tab ? tab->root : NULL;
}
