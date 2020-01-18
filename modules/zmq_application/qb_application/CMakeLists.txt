cmake_minimum_required(VERSION 2.8.3)
set(PROJ_NAME qb_application)
project(${PROJ_NAME})

add_definitions(-std=c++11)

find_package(Klepsydra REQUIRED)
find_package(KlepsydraRobotics REQUIRED)
find_package(cppzmq REQUIRED)

set(MAIN_INSTALL_PREFIX $ENV{HOME}/install)
set(THIRDPARTIES_PATH $ENV{HOME}/thirdparties)

set(QUEENBEESVC_LIBRARY queen_bee_svc)
set(ZMQ_UTILS_LIBRARY zmq_utils)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${${PROJ_MAIN_NAME}_PATH_MAIN}/modules/services/queen_bee_svc/include)
include_directories(${${PROJ_MAIN_NAME}_PATH_MAIN}/modules/services/robo_bee_svc/include)
include_directories(${${PROJ_MAIN_NAME}_PATH_MAIN}/modules/zmq_application/common)
include_directories(${${PROJ_MAIN_NAME}_PATH_MAIN}/modules/zmq_application/zmq_utils/include)
include_directories(${KLEPSYDRA_INCLUDE_DIRS})
include_directories(
  ${${PROJ_MAIN_NAME}_PATH_MAIN}/modules/codegen/gen/melissa_codegen/poco/include
)

file(GLOB ${PROJ_NAME}_HEADERS "include/*.h")
file(GLOB ${PROJ_NAME}_SRC "src/*.cpp")

link_directories(${${PROJ_MAIN_NAME}_PATH_LIBS})

add_executable(${PROJ_NAME} ${${PROJ_NAME}_SRC} ${${PROJ_NAME}_HEADERS})

target_link_libraries(
    ${PROJ_NAME}
    gtest_main
    atomic
    yaml-cpp
    ${KLEPSYDRA_CORE_LIBRARIES}
    ${KLEPSYDRA_ROBOTICS_CORE_LIBRARIES}
    kpsr_vision_ocv
    kpsr_admin
    kpsr_admin_system
    kpsr_socket_interface
    libzmq
    ${ZMQ_UTILS_LIBRARY}
    ${QUEENBEESVC_LIBRARY}
    ${OpenCV_LIBS}
)

install(
)