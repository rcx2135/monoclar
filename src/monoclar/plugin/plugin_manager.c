#include "plugin_manager_private.h"
#include "plugin_private.h"

#include <dirent.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

static char *join_path(const char *directory, const char *name) {
  size_t directory_length = strlen(directory);
  size_t name_length = strlen(name);

  size_t size = directory_length + name_length + 2;
  char *path = malloc(size);

  if (path) {
    snprintf(path, size, "%s/%s", directory, name);
  }

  return path;
}

monoclar_plugin_manager_t *monoclar_plugin_manager_create(void) {
  return calloc(1, sizeof(monoclar_plugin_manager_t));
}

size_t monoclar_plugin_manager_get_count(const monoclar_plugin_manager_t *manager) {
  return manager ? manager->count : 0;
}

monoclar_plugin_t *monoclar_plugin_manager_get_at(
    const monoclar_plugin_manager_t *manager, size_t index) {
  if (!manager || index >= manager->count) {
    return NULL;
  }

  return manager->plugins[index];
}

bool monoclar_plugin_manager_register(monoclar_plugin_manager_t *manager,
                                      monoclar_plugin_t *plugin) {
  if (!manager || !plugin || plugin->owner) {
    return false;
  }

  for (size_t i = 0; i < manager->count; i++) {
    if (strcmp(manager->plugins[i]->id, plugin->id) == 0) {
      return false;
    }
  }

  if (manager->count == manager->capacity) {
    size_t limit = SIZE_MAX / sizeof(monoclar_plugin_t *);

    if (manager->capacity > limit / 2) {
      return false;
    }

    size_t capacity = manager->capacity == 0 ? 4 : manager->capacity * 2;
    monoclar_plugin_t **plugins =
        realloc(manager->plugins, capacity * sizeof(monoclar_plugin_t *));

    if (!plugins) {
      return false;
    }

    manager->plugins = plugins;
    manager->capacity = capacity;
  }

  manager->plugins[manager->count++] = plugin;
  plugin->owner = manager;



  return true;
}

static bool discover_plugin(monoclar_plugin_manager_t *manager,
                            const char *directory, const char *id) {
  struct stat info;

  if (stat(directory, &info) != 0) {
    return false;
  }

  if (!S_ISDIR(info.st_mode)) {
    return true;
  }

  char *main_path = join_path(directory, "main.lua");

  if (!main_path) {
    free(main_path);
    return false;
  }

  monoclar_plugin_type_t type =
      MONOCLAR_PLUGIN_LUA; // assume lua, native comes later.

  monoclar_plugin_t *plugin = monoclar_plugin_create(id, directory, type);
  if (!plugin) {
    return false;
  }

  if (!monoclar_plugin_manager_register(manager, plugin)) {
    monoclar_plugin_destroy(plugin);
    return false;
  }

  return true;
}

void monoclar_plugin_manager_scan_directory(monoclar_plugin_manager_t *manager,
                                            const char *directory) {
  if (!manager || !directory || !*directory) {
    return;
  }

  DIR *dir = opendir(directory);
  if (!dir) {
    fprintf(stderr, "Cannot scan '%s': %s\n", directory, strerror(errno));
    return;
  }

  for (;;) {

    struct dirent *entry = readdir(dir);

    if (!entry) {
      break;
    }

    if (entry->d_name[0] == '.') {
      continue;
    }

    char *path = join_path(directory, entry->d_name);
    if (!path) {

      break;
    }

    discover_plugin(manager, path, entry->d_name);

    free(path);
  }

  closedir(dir);
}
