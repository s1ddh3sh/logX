include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(logger_tests
    logger_tests.cpp
)

target_link_libraries(logger_tests
    PRIVATE
        logX
        GTest::gtest_main
        pthread
)

target_include_directories(logger_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
        ${CMAKE_CURRENT_SOURCE_DIR}/../src
)


include(GoogleTest)
gtest_discover_tests(logger_tests)