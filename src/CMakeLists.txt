add_executable(${PROJECT_NAME}
  main.c
  monoclar/monoclar.c

  monoclar/display.c
  monoclar/ui/screen.c
   monoclar/lua/runtime.c
   monoclar/lua/ui.c
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

target_link_libraries(${PROJECT_NAME} PRIVATE lvgl lua luavgl)
