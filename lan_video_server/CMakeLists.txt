cmake_minimum_required(VERSION 3.10.2)

project(lan_video_server)

SET(CONAN_DISABLE_CHECK_COMPILER true)

include(${CMAKE_BINARY_DIR}/../conan_project/build/conan_paths.cmake)
include(${CMAKE_BINARY_DIR}/../conan_project/build/conanbuildinfo.cmake)
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

# 注意大小写
find_package(gflags REQUIRED)
message(STATUS "gflags include dir >>>>> ${gflags_INCLUDE_DIR}")
message(STATUS "gflags lib dir >>>>> ${gflags_LIB_DIRS}")

find_package(glog REQUIRED)
if(NOT glog_FOUND)
    message(FATAL_ERROR "glog is not found")
endif()
message(STATUS "glog include dir >>>>> ${glog_INCLUDE_DIR}")
message(STATUS "glog lib dir >>>>> ${glog_LIB_DIRS}")

# 注意大小写
find_package(fastdds REQUIRED)
if(NOT fastdds_FOUND)
    message(FATAL_ERROR "fastdds is not found")
endif()
message(STATUS "fastdds include dir >>>>> ${fastdds_INCLUDES}")
message(STATUS "fastdds lib dir >>>>> ${fastdds_LIB_DIRS}")

# set(LIB_PATH "-Wl,--rpath=${CONAN_LIB_DIRS_JSONCPP}:${glog_LIB_DIRS}:${gflags_LIB_DIRS}")
# set(LIB_PATH "-Wl,--rpath=/home/code/LAN_video_share/lan_video_server/lib/")
# set(LIB_PATH "-Wl,--rpath=/home/code/LAN_video_share/lan_video_server/lib")

# Default to C99
if(NOT CMAKE_C_STANDARD)
    set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
    set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wpedantic -Wno-reorder -Wno-variadic-macros -Wno-unused-parameter -Wno-unused-result -fdiagnostics-color=always")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O0 --coverage -fprofile-arcs -ftest-coverage -Wno-reorder ${LIB_PATH}")
set(CMAKE_C_FLAGS " -g -O0 --coverage -fprofile-arcs -ftest-coverage -Wno-reorder")

include_directories(
    include
    include/Utils
    fastdds_test/src
    ${CONAN_INCLUDE_DIRS_JSONCPP}
)

link_directories(
    ${CONAN_LIB_DIRS_JSONCPP}
)

set(MAIN_EXEC_NAME lan_video_server)

# 获得指定目录下所有源文件
aux_source_directory(src source_list)
aux_source_directory(fastdds_test/src fast_source_list)

add_executable(${MAIN_EXEC_NAME}
    ${source_list}
    ${fast_source_list}
)

target_link_libraries(
    ${MAIN_EXEC_NAME}
    pthread
    jsoncpp
    gflags
    glog
    fastrtps
    fastcdr
)

install(TARGETS
    ${MAIN_EXEC_NAME}
    DESTINATION ${CMAKE_BINARY_DIR}/../bin
)

# 将依赖的第三方库so全部复制到lib目录下，运行时在该目录下查找
add_custom_target(CopyTask
    # COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/config ${CMAKE_CURRENT_SOURCE_DIR}/etc
    COMMAND ${CMAKE_COMMAND} -E copy ${gflags_LIB_DIRS}/*so* ../lib
    COMMAND ${CMAKE_COMMAND} -E copy ${glog_LIB_DIRS}/*so* ../lib
    COMMAND ${CMAKE_COMMAND} -E copy ${CONAN_LIB_DIRS_GTEST}/*so* ../lib
    COMMAND ${CMAKE_COMMAND} -E copy ${CONAN_LIB_DIRS_JSONCPP}/*so* ../lib
    COMMAND ${CMAKE_COMMAND} -E copy ${fastdds_LIB_DIRS}/*so* ../lib
)

# add_subdirectory(test)