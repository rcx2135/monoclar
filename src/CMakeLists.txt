add_executable(${PROJECT_NAME}
  main.c
  monoclar/monoclar.c

  monoclar/ui/screen.c
  monoclar/ui/tab.c
  monoclar/ui/tab_manager.c
  monoclar/plugin/plugin.c
  monoclar/plugin/plugin_manager.c

  monoclar/lua/runtime.c
  monoclar/lua/ui.c
  monoclar/lua/tabs.c
)

target_include_directories(${PROJECT_NAME} PRIVATE
  "${PROJECT_SOURCE_DIR}/include/monoclar"
)

if(MONOCLAR_DISPLAY STREQUAL "wayland")
  target_sources(monoclar PRIVATE drivers/display/wayland.c)
elseif(MONOCLAR_DISPLAY STREQUAL "drm")
  target_sources(monoclar PRIVATE drivers/display/drm.c)
else()
  message(FATAL_ERROR "Unsupported display backend: ${MONOCLAR_DISPLAY}")
endif()

if(MONOCLAR_INPUT STREQUAL "evdev")
   find_package(PkgConfig REQUIRED)
   pkg_check_modules(LIBEVDEV REQUIRED IMPORTED_TARGET libevdev)

   target_sources(monoclar PRIVATE drivers/input/evdev.c)
   target_link_libraries(monoclar PRIVATE PkgConfig::LIBEVDEV)
 else()
   message(FATAL_ERROR "Unsupported input backend: ${MONOCLAR_INPUT}")
 endif()

target_link_libraries(${PROJECT_NAME} PRIVATE lvgl lua luavgl)
