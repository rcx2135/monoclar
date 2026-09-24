#pragma once

#include <plugin/plugin.h>
#include <plugin/plugin_manager.h>
#include <stddef.h>

struct monoclar_plugin_manager {
  monoclar_plugin_t **plugins;
  size_t count;
  size_t capacity;
};
