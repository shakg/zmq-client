cmake_minimum_required(VERSION 3.10)
project(zmq_tui_app CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FindPkgConfig)
pkg_check_modules(ZMQ REQUIRED libzmq)

add_executable(zmq_tui_app
    src/main.cpp
)

target_link_libraries(zmq_tui_app
    ${ZMQ_LIBRARIES}
    ncursesw
    pthread
)

target_include_directories(zmq_tui_app PRIVATE
    ${ZMQ_INCLUDE_DIRS}
)
