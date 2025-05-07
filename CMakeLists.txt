cmake_minimum_required(VERSION 3.7.2)
project(BS2025 C)

set(CMAKE_C_STANDARD 99)

add_executable(BS2025 main.c
        main.h
        keyValStore.cpp
        keyValStore.c
        keyValStore.h
        sub.c
        sub.h
)
