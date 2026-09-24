
#include "plugin_private.h"

#include <stdlib.h>
#include <string.h>

monoclar_plugin_t *monoclar_plugin_create(const char *id, const char *directory,
                                          monoclar_plugin_type_t type) {
  if (!id || !*id || !directory || !*directory) {
    return NULL;
  }

  if (type != MONOCLAR_PLUGIN_LUA) { //  && type != MONOCLAR_PLUGIN_NATIVE
    return NULL;
  }

  monoclar_plugin_t *plugin = calloc(1, sizeof(monoclar_plugin_t));
  if (!plugin) {
    return NULL;
  }

  plugin->id = strdup(id);
  plugin->directory = strdup(directory);
  plugin->type = type;

  if (!plugin->id || !plugin->directory) {
    monoclar_plugin_destroy(plugin);
    return NULL;
  }

  return plugin;
}

void monoclar_plugin_destroy(monoclar_plugin_t *plugin) {
  if (!plugin || plugin->owner) {
    return;
  }

  free(plugin->id);
  free(plugin->directory);
  free(plugin);
}
