#include "tabs_private.h"

#include <stdlib.h>
#include <string.h>

monoclar_tab_t *monoclar_tab_create(const char *id, const char *name,
                                  monoclar_tab_create_fn create,
                                  monoclar_tab_update_fn update,
                                  monoclar_tab_destroy_fn destroy,
                                  void *userdata)
{
  if (!id || !*id || !name) {
    return NULL;
  }

  monoclar_tab_t *tab = malloc(sizeof(monoclar_tab_t));
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

  tab->create = create;
  tab->update = update;
  tab->destroy = destroy;
  tab->userdata = userdata;
  return tab;
}

void monoclar_tab_destroy(monoclar_tab_t *tab)
{
  if (!tab || tab->owner) {
    return;
  }


  if (tab->destroy) {
    tab->destroy(tab, tab->userdata);
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

void monoclar_tab_request_update(monoclar_tab_t *tab)
{
  if (tab) {
    tab->update_requested = true;
  }
}
