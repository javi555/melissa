set(PROJ_NAME queen_bee_svc)
project(${PROJ_NAME})

set( ${PROJ_MAIN_NAME}_PATH_INSTALL )

# Include directories with headers
# ---------------------------------------------------#
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${${PROJ_MAIN_NAME}_PATH_MAIN}/modules/include)
include_directories(${KLEPSYDRA_INCLUDE_DIRS})
include_directories(
  ${${PROJ_MAIN_NAME}_PATH_MAIN}/modules/codegen/gen/melissa_codegen/poco/include
)

# Source
# ---------------------------------------------------#
file(GLOB ${PROJ_NAME}_SRC "src/*.cpp")
file(GLOB ${PROJ_NAME}_HEADERS "include/*.h")
file(GLOB ${PROJ_NAME}_GEN_HEADERS
     "../codegen/gen/melissa_codegen/poco/include/*.h")

# Create Library
# ---------------------------------------------------#
add_library(
  ${PROJ_NAME}
  ${${PROJ_MAIN_NAME}_LIB_TYPE}
  ${${PROJ_NAME}_HEADERS}
  ${${PROJ_NAME}_SRC}
  ${${PROJ_NAME}_GEN_HEADERS}
)

# Link libraries with Project
# ---------------------------------------------------#
link_directories(${${PROJ_MAIN_NAME}_PATH_LIBS})

target_link_libraries(
  ${PROJ_NAME}
  ${KLEPSYDRA_CORE_LIBRARIES}
  ${KLEPSYDRA_ROBOTICS_LIBRARIES}
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
install(
  FILES ${${PROJ_NAME}_GEN_HEADERS}
  DESTINATION "${${PROJ_MAIN_NAME}_PATH_INSTALL}/include/${PROJ_NAME}"
)