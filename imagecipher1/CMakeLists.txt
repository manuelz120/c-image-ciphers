cmake_minimum_required(VERSION 3.8)
project(imagecipher1)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        debug.h
        imagecipher1.c
        imagecipher1.h
        main.c
        Makefile)

add_executable(imagecipher1 ${SOURCE_FILES})
target_link_libraries(imagecipher1 m)