# cmake_minimum_required(VERSION 3.10.2)
project(test_project)

# 注意大小写
find_package(GTest REQUIRED)
if(NOT GTest_FOUND)
    message(FATAL_ERROR "GTest is not found")
endif()
message(STATUS "gtest include dir >>>>> ${CONAN_INCLUDE_DIRS_GTEST}")
message(STATUS "gtest lib dir >>>>> ${CONAN_LIB_DIRS_GTEST}")

# 注意大小写
find_package(jsoncpp REQUIRED)
find_package(gflags REQUIRED)
find_package(glog REQUIRED)

# Default to C99
if(NOT CMAKE_C_STANDARD)
    set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "-Wall  -Wextra -Wpedantic -Wno-reorder -Wno-variadic-macros -Wno-unused-parameter  -Wno-unused-result -fdiagnostics-color=always")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 --coverage -fprofile-arcs -ftest-coverage -Wno-reorder ${LIB_PATH}")
set(CMAKE_C_FLAGS " -g -O0 --coverage -fprofile-arcs -ftest-coverage -Wno-reorder")

include_directories(
    include
)

link_directories(
    ${CONAN_LIB_DIRS_JSONCPP}
)

set(test_exec lan_video_test)
# 获得制定目录下所有源文件
aux_source_directory(src test_source)

add_executable(${test_exec}
    ${test_source}
)

target_link_libraries(
    ${test_exec}
    pthread
    gtest
)

install(TARGETS
    ${test_exec}
    DESTINATION ${CMAKE_BINARY_DIR}/../bin
)