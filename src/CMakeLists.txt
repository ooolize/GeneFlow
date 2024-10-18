file(GLOB_RECURSE SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp)

set(SOURCE ${SRC_FILES})

find_package(fmt  REQUIRED)
add_executable(ZhouBoTong ${SOURCE})

target_include_directories(ZhouBoTong PUBLIC include)
target_link_libraries(ZhouBoTong PUBLIC   fmt::fmt pthread)

set_target_properties(ZhouBoTong PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
