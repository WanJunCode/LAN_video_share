cmake_minimum_required(VERSION 3.10.2)
project(zmq_demo_project)

SET(CONAN_DISABLE_CHECK_COMPILER true)
include(/home/code/LAN_video_share/lan_video_server/conan_project/build/conan_paths.cmake)
include(/home/code/LAN_video_share/lan_video_server/conan_project/build/conanbuildinfo.cmake)
conan_basic_setup()

# 注意大小写
find_package(ZeroMQ REQUIRED)
if(NOT ZeroMQ_FOUND)
    message(FATAL_ERROR "ZeroMQ is not found")
endif()
message(STATUS "ZeroMQ include dir >>>>> ${ZeroMQ_INCLUDES}")
message(STATUS "ZeroMQ lib dir >>>>> ${ZeroMQ_LIB_DIRS}")

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

link_directories(
    ${ZeroMQ_LIB_DIRS}
)

# send
set(publishe_exec zmq_server)
add_executable(${publishe_exec}
    send.c
    comm.c
)
target_link_libraries(
    ${publishe_exec}
    zmq
)

# receiver
set(subscriber_exec zmq_client)
add_executable(${subscriber_exec}
    recv.c
    comm.c
)
target_link_libraries(
    ${subscriber_exec}
    zmq
)
