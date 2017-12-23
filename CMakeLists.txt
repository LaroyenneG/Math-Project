cmake_minimum_required(VERSION 3.9)
project(Math_Project)

set(CMAKE_CXX_STANDARD 11)

add_executable(Math_Project
        libdebug.c
        libdebug.h
        libmat.c
        libmat.h
        pendule_22.c)

target_link_libraries(Math_Project m)