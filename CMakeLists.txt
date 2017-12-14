cmake_minimum_required(VERSION 3.9)
project(Math_Project)

set(CMAKE_CXX_STANDARD 11)

add_executable(Math_Project
        libmat-init/libdebug.c
        libmat-init/libdebug.h
        libmat-init/libmat.c
        libmat-init/libmat.h
        libmat-init/pend0.c
        libmat-init/pendflot.c)
