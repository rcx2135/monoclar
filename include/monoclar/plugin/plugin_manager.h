#pragma once

#include <plugin/plugin.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct monoclar_plugin_manager monoclar_plugin_manager_t;

monoclar_plugin_manager_t *monoclar_plugin_manager_create(void);

size_t monoclar_plugin_manager_get_count(const monoclar_plugin_manager_t *manager);

monoclar_plugin_t *monoclar_plugin_manager_get_at(
    const monoclar_plugin_manager_t *manager, size_t index);

bool monoclar_plugin_manager_register(monoclar_plugin_manager_t *manager,
                                      monoclar_plugin_t *plugin);

void monoclar_plugin_manager_scan_directory(monoclar_plugin_manager_t *manager,
                                            const char *directory);
