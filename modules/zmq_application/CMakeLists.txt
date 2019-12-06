
cmake_minimum_required(VERSION 2.8.3)
project(zmq_application)

add_definitions(-std=c++11)

find_package(Klepsydra REQUIRED)

set(MAIN_INSTALL_PREFIX $ENV{HOME}/install)
set(THIRDPARTIES_PATH $ENV{HOME}/thirdparties)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${${PROJ_MAIN_NAME}_PATH_MAIN}/modules/services/queen_bee_svc/include)
include_directories(${KLEPSYDRA_INCLUDE_DIRS})


#target_link_libraries()

install(
)
