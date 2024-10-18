file(GLOB_RECURSE TEST_FILES  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

set(TEST_SOURCES ${TEST_FILES})

find_package(fmt  REQUIRED)
find_package(GTest  REQUIRED)
add_executable(ZhouBoTongTest ${TEST_SOURCES})

target_link_libraries(ZhouBoTongTest PUBLIC  gtest::gtest fmt::fmt)
set_target_properties(ZhouBoTongTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)