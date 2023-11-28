find_package(GTest CONFIG REQUIRED)
file(GLOB_RECURSE TEST_SOURCES "*.cpp")
add_executable(${PROJECT_NAME}_test ${TEST_SOURCES})
target_link_libraries(
    ${PROJECT_NAME}_test PRIVATE
    ${PROJECT_NAME}_lib 
    GTest::gtest
    GTest::gtest_main
)