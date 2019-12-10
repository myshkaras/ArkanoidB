cmake_minimum_required(VERSION 3.14)
project(Arkanoid)

set(CMAKE_CXX_STANDARD 17)

add_executable(Arkanoid main.cpp canvas.cpp canvas.h ArkObjects.h drawingPrimitives.h ArkObjects.cpp pugixml.hpp  pugixml.cpp  pugiconfig.hpp)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtkmm-3.0)

include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})

add_definitions(${GTK3_CFLAGS_OTHER})

target_link_libraries(Arkanoid ${GTK3_LIBRARIES})