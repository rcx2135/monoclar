set(LV_BUILD_CONF_PATH
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/lvgl/wayland.h
    CACHE STRING "" FORCE
)
set(LV_BUILD_SET_CONFIG_OPTS ON CACHE BOOL
     "" FORCE)

add_subdirectory(
    ${CMAKE_CURRENT_SOURCE_DIR}/external/lvgl
)
