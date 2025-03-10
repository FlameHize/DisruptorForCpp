cmake_minimum_required(VERSION 2.8)
project(DisruptorForCpp)

option(FAST "Whether to set build type release and open -Ofast" OFF)
if(FAST)
    set(CMAKE_BUILD_TYPE RELEASE)
    set(CMAKE_CXX_FLAGS "-std=c++11 -fno-rtti -g -Wall -Ofast")
	message("Open Fast Build")
else()
    set(CMAKE_BUILD_TYPE DEBUG)
    set(CMAKE_CXX_FLAGS "-std=c++11 -g -fno-rtti -Wall")
	message("Close Fast Build")
endif()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/test)
include_directories(${PROJECT_SOURCE_DIR}/benchmark)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(benchmark)

#target_link_directories 为特定的目标添加连接的目录
#target_include_directories 为特定的目标添加头文件路径(用于工程项目内搜索)
