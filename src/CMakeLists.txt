add_executable(${PROJECT_NAME}
      main.cpp

  )

if(MONOCLAR_DISPLAY STREQUAL "wayland")
  target_sources(monoclar PRIVATE drivers/display/wayland.cpp)
else()
  message(FATAL_ERROR "Unsupported display backend: ${MONOCLAR_DISPLAY}")
endif()

target_link_libraries(${PROJECT_NAME} PRIVATE lvgl)
