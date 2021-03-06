
cmake_minimum_required(VERSION 2.8.3)
set(PROJ_NAME rb_application)
project(${PROJ_NAME})

add_definitions(-std=c++11)

find_package(Klepsydra REQUIRED)
find_package(cppzmq REQUIRED)

set(MAIN_INSTALL_PREFIX $ENV{HOME}/install)
set(THIRDPARTIES_PATH $ENV{HOME}/thirdparties)

set(ROBOBEESVC_LIBRARY robo_bee_svc)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${${PROJ_MAIN_NAME}_PATH_MAIN}/modules/services/robo_bee_svc/include)
include_directories(${${PROJ_MAIN_NAME}_PATH_MAIN}/modules/zmq_application/common)
include_directories(${KLEPSYDRA_INCLUDE_DIRS})
include_directories(
  ${${PROJ_MAIN_NAME}_PATH_MAIN}/modules/codegen/gen/melissa_codegen/poco/include
)
include_directories(
  ${${PROJ_MAIN_NAME}_PATH_MAIN}/modules/zmq_application/zmq_utils/include
)

file(GLOB ${PROJ_NAME}_SRC "src/*.cpp")
file(GLOB ${PROJ_NAME}_HEADERS
    "include/*.h"
)

link_directories(${${PROJ_MAIN_NAME}_PATH_LIBS})

add_executable(${PROJ_NAME} ${${PROJ_NAME}_HEADERS} ${${PROJ_NAME}_SRC})

target_link_libraries(
    ${PROJ_NAME}
    gtest_main
    atomic
    yaml-cpp
    kpsr_core_yaml 
    kpsr_vision_ocv 
    kpsr_socket_interface
    kpsr_admin
    kpsr_admin_system
    libzmq
    ${ROBOBEESVC_LIBRARY}
    ${OpenCV_LIBS}
)

install(
)