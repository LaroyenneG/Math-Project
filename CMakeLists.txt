cmake_minimum_required(VERSION 3.9)
project(Math_Project)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 11)


add_executable(Math_Project
        libdebug.c
        libdebug.h
        libmat.c
        libmat.h
        pendule_22.c view.c view.h delete-me.h)

target_link_libraries(Math_Project ${SDL2_LIBRARIES} m)
