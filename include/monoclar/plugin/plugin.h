#pragma once

typedef struct monoclar_plugin monoclar_plugin_t;

typedef enum {
  MONOCLAR_PLUGIN_LUA,
  MONOCLAR_PLUGIN_NATIVE
} monoclar_plugin_type_t;

monoclar_plugin_t *monoclar_plugin_create(const char *id, const char *directory,
                                          monoclar_plugin_type_t type);
void monoclar_plugin_destroy(monoclar_plugin_t *plugin);
