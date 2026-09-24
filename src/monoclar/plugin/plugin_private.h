#pragma once

#include <plugin/plugin.h>
#include <plugin/plugin_manager.h>

struct monoclar_plugin {
  char *id;
  char *directory;
  monoclar_plugin_type_t type;
  monoclar_plugin_manager_t *owner;
};
