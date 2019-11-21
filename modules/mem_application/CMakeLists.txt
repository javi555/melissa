cmake_minimum_required(VERSION 2.8.3)
set(PROJ_NAME mem_application)
project(${PROJ_NAME})

add_definitions(-std=c++11)

find_package(Klepsydra REQUIRED)

set(MAIN_INSTALL_PREFIX $ENV{HOME}/install)
set(THIRDPARTIES_PATH $ENV{HOME}/thirdparties)

set(ROBOBEESVC_LIBRARY robo_bee_svc)
set(QUEENBEESVC_LIBRARY queen_bee_svc)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${${PROJ_MAIN_NAME}_PATH_MAIN}/modules/services/queen_bee_svc/include)
include_directories(${${PROJ_MAIN_NAME}_PATH_MAIN}/modules/services/robo_bee_svc/include)

include_directories(${KLEPSYDRA_INCLUDE_DIRS})

file(GLOB ${PROJ_NAME}_SRC "src/*.cpp")

link_directories(${${PROJ_MAIN_NAME}_PATH_LIBS})

add_executable(${PROJ_NAME} ${${PROJ_NAME}_SRC})

target_link_libraries(
    ${PROJ_NAME}
    gtest_main
    atomic
    ${ROBOBEESVC_LIBRARY}
    ${QUEENBEESVC_LIBRARY}
)

install(
)
