cmake_minimum_required(VERSION 3.14)
project(logX LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_compile_options(-Wall -pthread)


add_library(logX
    src/logger.cpp
    src/config/config.cpp
)

target_include_directories(logX
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)


add_executable(example
    example/example.cpp
)
target_link_libraries(example PRIVATE logX)

set_target_properties(example PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
)

# Tests
enable_testing()
add_subdirectory(tests)