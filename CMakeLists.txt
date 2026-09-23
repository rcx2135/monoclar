cmake_minimum_required(VERSION 3.24)
project(monoclar LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(MONOCLAR_DISPLAY "wayland" CACHE STRING "Display backend to use.")

include(cmake/dependencies.cmake)
add_subdirectory(src)
