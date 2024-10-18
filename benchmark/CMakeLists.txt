file(GLOB_RECURSE BENCHMARK_FILES  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

set(BENCHMARK_SOURCES ${BENCHMARK_FILES})

find_package(benchmark REQUIRED)
find_package(fmt  REQUIRED)
add_executable(ZhouBoTongBenchmark ${BENCHMARK_SOURCES})

target_link_libraries(ZhouBoTongBenchmark PUBLIC fmt::fmt benchmark::benchmark_main)

set_target_properties(ZhouBoTongBenchmark PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)