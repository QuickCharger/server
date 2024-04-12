CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)

# FRAME_INCLUDE
SET(FRAME_INCLUDE		"${CMAKE_CURRENT_SOURCE_DIR}/frame")

# PROJECT_PROTOBUF_INCLUDE
SET(PROJECT_PROTOBUF_INCLUDE	"${CMAKE_CURRENT_SOURCE_DIR}")

add_subdirectory(ClientServer)
#add_subdirectory(AuthServer)
#add_subdirectory(DatabaseServer)
add_subdirectory(DustbinServer)