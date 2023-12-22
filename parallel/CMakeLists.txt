file(GLOB_RECURSE parallel_SRC *.cpp *.cu)
add_executable(parallel ${parallel_SRC})
target_include_directories(parallel PUBLIC ./include)