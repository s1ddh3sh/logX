cmake_minimum_required(VERSION 3.14)
project(logX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -pthread)

add_executable(logger
    logX.cpp
    src/logger.cpp
    src/hooks/log_hook.h
    src/hooks/file_hook.h
    src/hooks/console_hook.h
    src/config/config.cpp
)
enable_testing()
add_subdirectory(tests)