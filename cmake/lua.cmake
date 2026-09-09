set(lua_source_dir "${PROJECT_SOURCE_DIR}/external/lua/src")

set(lua_sources
    lapi.c lcode.c lctype.c ldebug.c ldo.c ldump.c
    lfunc.c lgc.c llex.c lmem.c lobject.c lopcodes.c
    lparser.c lstate.c lstring.c ltable.c ltm.c
    lundump.c lvm.c lzio.c
    lauxlib.c lbaselib.c lcorolib.c ldblib.c liolib.c
    lmathlib.c loadlib.c loslib.c lstrlib.c ltablib.c
    lutf8lib.c linit.c
)

list(TRANSFORM lua_sources PREPEND "${lua_source_dir}/")

add_library(lua ${lua_sources})


target_compile_definitions(lua PRIVATE LUA_USE_LINUX)
target_link_libraries(lua PRIVATE ${CMAKE_DL_LIBS})
