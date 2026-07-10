cmake_minimum_required(VERSION 3.16)

project(JobTrackerCLI C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(JobTrackerCLI
    src/main.c
    src/application.c
    src/file.c
    src/menu.c
    src/stats.c
)

target_include_directories(JobTrackerCLI PRIVATE include)

target_compile_options(JobTrackerCLI PRIVATE
    -Wall
    -Wextra
    -Wpedantic
)