file(GLOB_RECURSE TEST_FILES  ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

set(TEST_SOURCES ${TEST_FILES})

find_package(fmt  REQUIRED)
find_package(GTest  REQUIRED)
find_package(nlohmann_json  REQUIRED)
add_executable(GeneFlowTest ${TEST_SOURCES})

target_link_libraries(GeneFlowTest PUBLIC nlohmann_json::nlohmann_json  gtest::gtest fmt::fmt)
set_target_properties(GeneFlowTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# 将data目录拷贝到编译目录
add_custom_command(TARGET GeneFlowTest POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/data $<TARGET_FILE_DIR:GeneFlowTest>/data)