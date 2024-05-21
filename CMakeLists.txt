cmake_minimum_required(VERSION 3.25)

project(gds VERSION 0.0.1 LANGUAGES CXX C ASM)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(GDS_BUILD_TESTS "Build tests" ON)
option(GDS_BUILD_BENCHMARKS "Build benchmarks" ON)
option(GDS_USE_CUDA "Use CUDA" OFF)

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

if(MSVC)
    target_compile_options(${PROJECT_NAME}_lib PUBLIC /utf-8 /openmp)
else()
    target_compile_options(${PROJECT_NAME}_lib PUBLIC -fopenmp)
endif()

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

# target parallel
# add_subdirectory(parallel)