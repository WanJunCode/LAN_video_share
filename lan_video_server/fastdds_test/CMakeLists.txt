
cmake_minimum_required(VERSION 2.8.12)

project("generated_code")

SET(CONAN_DISABLE_CHECK_COMPILER true)
include(/home/code/LAN_video_share/lan_video_server/conan_project/build/conan_paths.cmake)
include(/home/code/LAN_video_share/lan_video_server/conan_project/build/conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find requirements
find_package(fastcdr REQUIRED)
find_package(fastdds REQUIRED)


message(STATUS "Configuring hello...")
add_library(hello_lib hello.cxx)
target_link_libraries(hello_lib fastcdr fastrtps)

add_executable(hello helloPubSubTypes.cxx helloPublisher.cxx helloSubscriber.cxx helloPubSubMain.cxx)
target_link_libraries(hello fastcdr fastrtps
        hello_lib )



