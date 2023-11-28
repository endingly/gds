find_package(benchmark CONFIG REQUIRED)
file(GLOB_RECURSE BENCHMARK_SOURCES "*.cpp")
add_executable(${PROJECT_NAME}_benchmark ${BENCHMARK_SOURCES})
target_link_libraries(
    ${PROJECT_NAME}_benchmark PRIVATE
    ${PROJECT_NAME}_lib 
    benchmark::benchmark 
    benchmark::benchmark_main
)