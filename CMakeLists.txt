cmake_minimum_required(VERSION 3.26)

project(gds VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(GDS_BUILD_TESTS "Build tests" ON)
option(GDS_BUILD_BENCHMARKS "Build benchmarks" ON)

file(GLOB_RECURSE SOURCES "src/*.cpp")
list(REMOVE_ITEM SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp")

# target gds lib for benchmark and tests
if(CMAKE_BUILD_TYPE MATCHES Debug)
    find_package(Backward REQUIRED)
endif()

find_package(Eigen3 CONFIG REQUIRED)
add_library(${PROJECT_NAME}_lib ${SOURCES})
target_include_directories(${PROJECT_NAME}_lib PUBLIC include)
target_link_libraries(
    ${PROJECT_NAME}_lib PUBLIC
    Eigen3::Eigen
    $<$<CONFIG:Debug>:Backward::Backward>
)

# target gds executable
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_lib)

# target tests
if(GDS_BUILD_TESTS)
    add_subdirectory(tests)
endif()

# target benchmarks
if(GDS_BUILD_BENCHMARKS)
    add_subdirectory(benchmark)
endif()