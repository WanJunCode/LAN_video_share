cmake_minimum_required(VERSION 3.10.2)

project(echo_server)

include(${CMAKE_BINARY_DIR}/../../conan_project/build/conan_paths.cmake)
include(${CMAKE_BINARY_DIR}/../../conan_project/build/conanbuildinfo.cmake)
conan_basic_setup()

# 注意大小写
find_package(GTest REQUIRED)
if(NOT GTest_FOUND)
    message(FATAL_ERROR "GTest is not found")
endif()
message(STATUS "gtest include dir >>>>> ${CONAN_INCLUDE_DIRS_GTEST}")
message(STATUS "gtest lib dir >>>>> ${CONAN_LIB_DIRS_GTEST}")

# 注意大小写
find_package(jsoncpp REQUIRED)
if(NOT jsoncpp_FOUND)
    message(FATAL_ERROR "jsoncpp is not found")
endif()

message(STATUS "jsoncpp include dir >>>>> ${CONAN_INCLUDE_DIRS_JSONCPP}")
message(STATUS "jsoncpp lib dir >>>>> ${CONAN_LIB_DIRS_JSONCPP}")

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

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 --coverage -fprofile-arcs -ftest-coverage -Wno-reorder")
set(CMAKE_C_FLAGS " -g -O0 --coverage -fprofile-arcs -ftest-coverage -Wno-reorder")

include_directories(include/)

set(MAIN_EXEC_NAME echo_server)

add_executable(${MAIN_EXEC_NAME}
    src/main.cpp
    src/Tcp_Server.cpp
    src/Timer.cpp
)

target_link_libraries(
    ${MAIN_EXEC_NAME}
    pthread
    gtest
)

install(TARGETS
    ${MAIN_EXEC_NAME}
    DESTINATION ${CMAKE_BINARY_DIR}/../bin
)