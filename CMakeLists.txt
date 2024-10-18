# mkdir build
# cd build
# cmake .. -DCMAKE_BUILD_TYPE=Release -G "MinGW Makefiles"  ; cmake --build . -j4
# clang-tidy .\src\command\command.cpp -p build -- -I F:\project\pattern\include
# rm -rf build/*; cmake -S . -B build; cmake --build  build -j4

# rm -rf build/*;conan install .  --build=missing ;cmake --preset conan-release ;cmake --build --preset conan-release
# rm -rf build/*; conan install . --build=missing --settings=build_type=Debug  ;cmake  --preset conan-debug -DCMAKE_BUILD_TYPE=Debug ;cmake --build --preset  conan-debug
# /usr/lib/linux-tools/6.8.0-40-generic/perf
cmake_minimum_required(VERSION 3.5)

# 如果windows下想使用g++ 需要编译时 cmake -G "MinGW Makefiles" ..
# 否则默认使用Visual Studio 17 2022 生成器
# 设置使用的编译器
set(CMAKE_CXX_COMPILER clang++)
# 设置c++标准
set(CMAKE_CXX_STANDARD 20)

# 设置项目名称
project(zhoubotong)

# 设置编译选项
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_definitions(-DDEBUG) # 定义DEBUG宏
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall --std=c++2a") # 带调试信息 启用警告
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -Wall --std=c++2a") # 优化编译
endif()


# 添加全局包含路径
include_directories(${PROJECT_SOURCE_DIR}/include  ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR})
# 添加全局共享库路径
link_directories(${PROJECT_SOURCE_DIR}/lib)

# 添加子目录
add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(benchmark)
# add_executable(pattern main.cpp)
