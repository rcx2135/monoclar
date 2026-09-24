#include "tabs.h"
#include "../ui/tab_private.h"

#include <lauxlib.h>
#include <luavgl.h>
#include <string.h>

#define TAB_META "monoclar.tab"

typedef struct {
  monoclar_tab_manager_t *manager;
  lua_State *L;
} lua_tabs_t;

typedef struct {
  monoclar_tab_t *tab;
  lua_tabs_t *tabs;
  int self_ref;
  bool destroying;
} lua_tab_t;

static const char tabs_key;

static void push_tabs(lua_State *L) {
  lua_rawgetp(L, LUA_REGISTRYINDEX, &tabs_key);
  lua_getiuservalue(L, -1, 1);
  lua_remove(L, -2);
}

static void pin_tab(lua_State *L, lua_tab_t *binding, int index) {
  if (binding->self_ref == LUA_NOREF) {
    lua_pushvalue(L, index);
    binding->self_ref = luaL_ref(L, LUA_REGISTRYINDEX);
  }
}

static void unpin_tab(lua_tab_t *binding) {
  luaL_unref(binding->tabs->L, LUA_REGISTRYINDEX, binding->self_ref);
  binding->self_ref = LUA_NOREF;
}

static lua_tab_t *check_tab(lua_State *L) {
  lua_tab_t *binding = luaL_checkudata(L, 1, TAB_META);
  if (!binding->tab) {
    luaL_error(L, "tab has been destroyed");
  }

  return binding;
}

static lua_tabs_t *check_manager(lua_State *L) {
  lua_tabs_t *tabs = lua_touserdata(L, lua_upvalueindex(1));
  if (!tabs->manager) {
    luaL_error(L, "tab bindings are closed");
  }
  return tabs;
}

static int tab_gc(lua_State *L) {
  lua_tab_t *binding = luaL_checkudata(L, 1, TAB_META);
  if (binding->tab && !binding->tab->owner && !binding->destroying) {
    pin_tab(L, binding, 1);
    monoclar_tab_destroy(binding->tab);
  }
  return 0;
}

static int push_root(lua_State *L) {
  lv_obj_t *root = lua_touserdata(L, 1);
  luavgl_add_lobj(L, root)->lua_created = false;
  return 1;
}

static int tab_index(lua_State *L) { // suffices for noiw but maybe have some kind of reflection if needed??? (if even possible in c)
  lua_tab_t *binding = check_tab(L);
  size_t length;
  const char *key =
      lua_type(L, 2) == LUA_TSTRING ? lua_tolstring(L, 2, &length) : NULL;
  if (key && length == 4 && memcmp(key, "name", 4) == 0) {
    lua_pushstring(L, binding->tab->name);
    return 1;
  }
  if (key && length == 2 && memcmp(key, "id", 2) == 0) {
    lua_pushstring(L, binding->tab->id);
    return 1;
  }
  if (!key || length != 4 || memcmp(key, "root", 4) != 0) {
    lua_pushnil(L);
    return 1;
  }

  lv_obj_t *root = monoclar_tab_get_root(binding->tab);
  if (!root) {
    lua_pushnil(L);
    return 1;
  }

  lua_State *main = binding->tabs->L;
  lua_pushcfunction(main, push_root);
  lua_pushlightuserdata(main, root);
  int status = lua_pcall(main, 1, 1, 0);
  if (L != main) {
    lua_xmove(main, L, 1);
  }
  if (status != LUA_OK) {
    return lua_error(L);
  }
  return 1;
}

static int tab_create(lua_State *L) {
  lua_tabs_t *tabs = check_manager(L);
  luaL_checktype(L, 1, LUA_TTABLE);
  lua_settop(L, 1);
  lua_getfield(L, 1, "id");
  lua_getfield(L, 1, "name");
  size_t id_length, name_length;
  const char *id = luaL_checklstring(L, 2, &id_length);
  const char *name = luaL_checklstring(L, 3, &name_length);
  if (!id_length || strlen(id) != id_length || strlen(name) != name_length) {
    return luaL_error(L, "tab id must be not empty");
  }

  lua_tab_t *binding = lua_newuserdatauv(L, sizeof(lua_tab_t), 1);
  *binding = (lua_tab_t){.tabs = tabs, .self_ref = LUA_NOREF};
  luaL_setmetatable(L, TAB_META);
  lua_pushvalue(L, lua_upvalueindex(1));
  lua_setiuservalue(L, 4, 1);

  binding->tab = monoclar_tab_create(id, name);
  if (!binding->tab) {
    return luaL_error(L, "cannot allocate tab");
  }
  push_tabs(L);
  lua_pushvalue(L, 4);
  lua_rawsetp(L, -2, binding->tab);
  lua_pop(L, 1);
  return 1;
}

static int tab_register(lua_State *L) {
  lua_tabs_t *tabs = check_manager(L);
  lua_tab_t *binding = check_tab(L);
  if (binding->tabs != tabs || binding->tab->owner || binding->destroying) {
    lua_pushboolean(L, false);
    return 1;
  }
  pin_tab(L, binding, 1);
  bool registered = monoclar_tab_manager_register(tabs->manager, binding->tab);
  if (!registered) {
    unpin_tab(binding);
  }
  lua_pushboolean(L, registered);
  return 1;
}

static int tab_unregister(lua_State *L) {
  lua_tabs_t *tabs = check_manager(L);
  lua_tab_t *binding = check_tab(L);
  bool removed = binding->tabs == tabs && !binding->destroying &&
                 binding->tab->owner == tabs->manager &&
                 monoclar_tab_manager_unregister(tabs->manager, binding->tab);
  if (removed) {
    unpin_tab(binding);
  }
  lua_pushboolean(L, removed);
  return 1;
}

static int tab_set_active(lua_State *L) {
  lua_tabs_t *tabs = check_manager(L);
  monoclar_tab_t *tab = NULL;
  if (!lua_isnoneornil(L, 1)) {
    lua_tab_t *binding = check_tab(L);
    if (binding->tabs != tabs || binding->tab->owner != tabs->manager ||
        binding->destroying) {
      lua_pushboolean(L, false);
      return 1;
    }
    tab = binding->tab;
  }
  lua_pushboolean(L, monoclar_tab_manager_set_active(tabs->manager, tab));
  return 1;
}

static int tab_get_active(lua_State *L) {
  lua_tabs_t *tabs = check_manager(L);
  monoclar_tab_t *tab = monoclar_tab_manager_get_active(tabs->manager);
  if (!tab) {
    lua_pushnil(L);
    return 1;
  }
  push_tabs(L);
  lua_rawgetp(L, -1, tab);
  return 1;
}

static int get_tabs(lua_State *L) // needs better implementation later
{
  lua_tabs_t *tabs = lua_touserdata(L, lua_upvalueindex(1));

  lua_newtable(L);
  int result = lua_gettop(L);
  lua_Integer index = 1;

  push_tabs(L);
  lua_pushnil(L);
  while (lua_next(L, -2)) {
    lua_tab_t *binding = lua_touserdata(L, -1);
    if (binding->tab && binding->tab->owner == tabs->manager) {
      lua_pushvalue(L, -1);
      lua_rawseti(L, result, index++);
    }
    lua_pop(L, 1);
  }
  lua_pop(L, 1);
  return 1;
}

static int tab_destroy(lua_State *L) {
  check_manager(L);
  lua_tab_t *binding = check_tab(L);
  if (binding->tab->owner) {
    return luaL_error(L, "unregister the tab before destroying it");
  }
  if (binding->destroying) {
    return luaL_error(L, "tab is being destroyed");
  }

  pin_tab(L, binding, 1);
  monoclar_tab_destroy(binding->tab);
  return 0;
}

static int luaopen_monoclar_tabs(lua_State *L) {
  static const luaL_Reg functions[] = {
      {"create", tab_create},
      {"register", tab_register},
      {"unregister", tab_unregister},
      {"set_active", tab_set_active},
      {"get_active", tab_get_active},
      {"get_tabs", get_tabs},

      {"destroy", tab_destroy},
      {NULL, NULL},
  };
  lua_newtable(L);
  lua_pushvalue(L, lua_upvalueindex(1));
  luaL_setfuncs(L, functions, 1);
  return 1;
}

void monoclar_lua_bind_tabs(lua_State *L, monoclar_tab_manager_t *manager) {
  luaL_newmetatable(L, TAB_META);
  lua_pushcfunction(L, tab_gc);
  lua_setfield(L, -2, "__gc");
  lua_pushcfunction(L, tab_index);
  lua_setfield(L, -2, "__index");
  lua_pushliteral(L, TAB_META);
  lua_setfield(L, -2, "__metatable");
  lua_pop(L, 1);

  lua_tabs_t *tabs = lua_newuserdatauv(L, sizeof(lua_tabs_t), 1);
  *tabs = (lua_tabs_t){.manager = manager};
  lua_rawgeti(L, LUA_REGISTRYINDEX, LUA_RIDX_MAINTHREAD);
  tabs->L = lua_tothread(L, -1);
  lua_pop(L, 1);

  lua_newtable(L);
  lua_newtable(L);
  lua_pushliteral(L, "v");
  lua_setfield(L, -2, "__mode");
  lua_setmetatable(L, -2);
  lua_setiuservalue(L, -2, 1);
  lua_pushvalue(L, -1);
  lua_rawsetp(L, LUA_REGISTRYINDEX, &tabs_key);

  luaL_getsubtable(L, LUA_REGISTRYINDEX, LUA_PRELOAD_TABLE);
  lua_pushvalue(L, -2);
  lua_pushcclosure(L, luaopen_monoclar_tabs, 1);
  lua_setfield(L, -2, "monoclar.tabs");
  lua_pop(L, 2);
}

// write cleanup logic later
