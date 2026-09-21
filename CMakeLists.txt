cmake_minimum_required(VERSION 3.24)
project(monoclar LANGUAGES C CXX)

set(MONOCLAR_DISPLAY "wayland" CACHE STRING "Display backend to use (wayland, x11, etc.)")

include(cmake/dependencies.cmake)
add_subdirectory(src)
