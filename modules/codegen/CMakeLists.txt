
set(PROJ_NAME ${PROJ_MAIN_NAME}_codegen)
project(${PROJ_NAME})

kpsreventgenerator(
  "${CMAKE_CURRENT_SOURCE_DIR}/kidl"
  "${CMAKE_CURRENT_SOURCE_DIR}/gen/${PROJ_NAME}"
  "."
  "False"
  "True"
  "True"
)

# Source
# ---------------------------------------------------#
file(GLOB_RECURSE ${PROJ_NAME}_HEADERS "gen/${PROJ_NAME}/poco/include/*.h")

file(COPY ${CONFIG_MAPPERS} DESTINATION ${CMAKE_SOURCE_DIR}/codegen/include/)