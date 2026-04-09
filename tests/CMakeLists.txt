enable_testing()

add_executable(
  hello_test
  hello_test.cc
)
target_link_libraries(hello_test
    PRIVATE GTest::gtest_main
    PRIVATE fmt::fmt
    PRIVATE naida
)
target_include_directories(hello_test
    PRIVATE ${PROJECT_SOURCE_DIR}/include
    PRIVATE ${PROJECT_SOURCE_DIR}/3rdparty
)

include(GoogleTest)
gtest_discover_tests(hello_test)