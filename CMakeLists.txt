cmake_minimum_required(VERSION 3.27)

project(gds VERSION 0.0.1 LANGUAGES CXX CUDA)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(GDS_BUILD_TESTS "Build tests" ON)
option(GDS_BUILD_BENCHMARKS "Build benchmarks" ON)

file(GLOB_RECURSE SOURCES "src/*.cpp")
list(
    REMOVE_ITEM SOURCES 
    "${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/backward.cpp"
)

# target gds lib for benchmark and tests
find_package(Eigen3 CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(GSL REQUIRED)
add_library(${PROJECT_NAME}_lib ${SOURCES})
target_include_directories(${PROJECT_NAME}_lib PUBLIC include)
target_link_libraries(
    ${PROJECT_NAME}_lib PUBLIC
    Eigen3::Eigen
    fmt::fmt
    GSL::gsl 
    GSL::gslcblas
)
target_compile_options(${PROJECT_NAME}_lib PUBLIC /utf-8 /openmp)

# target gds executable
find_package(Backward REQUIRED)
add_executable(${PROJECT_NAME} src/main.cpp src/backward.cpp)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}_lib Backward::Backward)

# target tests
if(GDS_BUILD_TESTS)
    add_subdirectory(tests)
endif()

# target benchmarks
if(GDS_BUILD_BENCHMARKS)
    add_subdirectory(benchmark)
endif()

# target parallel
add_subdirectory(parallel)