cmake_minimum_required(VERSION 3.14)
project(logX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -pthread)

include(FetchContent)

FetchContent_Declare(
	json
	URL https://github.com/nlohmann/json/releases/download/v3.12.0/json.tar.xz
)

FetchContent_MakeAvailable(json)

add_executable(logger
    logX.cpp
    src/logger.cpp
    src/hooks/log_hook.h
    src/hooks/file_hook.h
    src/hooks/console_hook.h
)

target_link_libraries(logger PRIVATE nlohmann_json::nlohmann_json)