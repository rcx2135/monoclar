
add_library(luavgl STATIC
    "${PROJECT_SOURCE_DIR}/external/luavgl/src/luavgl.c"
)

target_include_directories(luavgl PUBLIC
    "${PROJECT_SOURCE_DIR}/external/luavgl/src"
)

target_link_libraries(luavgl PUBLIC lua lvgl)
