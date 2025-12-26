cmake_minimum_required(VERSION 4.0)
project(overcooked_scheduler)

set(CMAKE_CXX_STANDARD 20)

add_executable(overcooked_scheduler
        Scheduler.cpp
        Scheduler.h
        main.cpp
)
