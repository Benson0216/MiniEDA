include(FetchContent)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.17.0.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(minieda_tests
    test_main.cpp
    test_entities.cpp
    test_design.cpp
    test_string_utils.cpp
)

target_link_libraries(minieda_tests
    PRIVATE
    GTest::gtest_main
)

target_include_directories(minieda_tests
    PRIVATE
    ${CMAKE_SOURCE_DIR}/src
)

include(GoogleTest)
gtest_discover_tests(minieda_tests)
