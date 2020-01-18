cmake_minimum_required(VERSION 2.8.3)
set(PROJ_NAME zmq_utils)
project(${PROJ_NAME})

find_package(cppzmq REQUIRED)

set(QUEENBEESVC_LIBRARY queen_bee_svc)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${${PROJ_MAIN_NAME}_PATH_MAIN}/modules/services/queen_bee_svc/include)
include_directories(${${PROJ_MAIN_NAME}_PATH_MAIN}/modules/zmq_application/common)
include_directories(${KLEPSYDRA_INCLUDE_DIRS})
include_directories(
  ${${PROJ_MAIN_NAME}_PATH_MAIN}/modules/codegen/gen/melissa_codegen/poco/include
)

file(GLOB ${PROJ_NAME}_SRC "src/*.cpp")
file(GLOB ${PROJ_NAME}_HEADERS "include/*.h")

add_library(
  ${PROJ_NAME}
  ${${PROJ_MAIN_NAME}_LIB_TYPE}
  ${${PROJ_NAME}_HEADERS}
  ${${PROJ_NAME}_SRC}
)

link_directories(${${PROJ_MAIN_NAME}_PATH_LIBS})

target_link_libraries(
  ${PROJ_NAME}
  ${KLEPSYDRA_CORE_LIBRARIES}
  gtest_main
  atomic
  yaml-cpp
  kpsr_core_yaml 
  kpsr_vision_ocv 
  kpsr_socket_interface
  kpsr_admin
  kpsr_admin_system
  ${OpenCV_LIBS}
)

# Install
# ---------------------------------------------------#
install(
  TARGETS ${PROJ_NAME} DESTINATION "${MAIN_INSTALL_PREFIX}/lib/"
)
install(
  FILES ${${PROJ_NAME}_HEADERS}
  DESTINATION "${MAIN_INSTALL_PREFIX}/include/${PROJ_NAME}"
)