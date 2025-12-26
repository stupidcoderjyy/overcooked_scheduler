cmake_minimum_required(VERSION 3.31.6)
project(overcooked_scheduler)

set(CMAKE_CXX_STANDARD 20)

add_executable(overcooked_scheduler
        Scheduler.cpp
        Scheduler.h
        main.cpp
)
