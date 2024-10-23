file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

set(SOURCE ${SRC_FILES})

find_package(fmt  REQUIRED)
find_package(nlohmann_json  REQUIRED)
add_executable(GeneFlow ${SOURCE})

target_include_directories(GeneFlow PUBLIC include)
target_link_libraries(GeneFlow PUBLIC nlohmann_json::nlohmann_json  fmt::fmt pthread)

set_target_properties(GeneFlow PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
