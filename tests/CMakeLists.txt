include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

enable_testing()
add_executable(logger_tests
    logger_tests.cpp
    ../src/logger.cpp
    ../src/hooks/file_hook.h
    ../src/hooks/console_hook.h
    ../src/config/config.cpp
)

target_include_directories(logger_tests PRIVATE ../src)

target_link_libraries(logger_tests
    GTest::gtest_main
    pthread
)

include(GoogleTest)
gtest_discover_tests(logger_tests)