cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(Server)

SET(LIBEVENT_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/third_part/libevent-2.0.22-stable/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/third_part/libevent-2.0.22-stable/include/event2" 
	"${CMAKE_CURRENT_SOURCE_DIR}/third_part/libevent-2.0.22-stable/WIN32-Code" 
	"${CMAKE_CURRENT_SOURCE_DIR}/third_part/libevent-2.0.22-stable/WIN32-Code/event2")
SET(LIBEVENT_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/third_part/libevent-2.0.22-stable/lib")
SET(LIBEVENT_LIB libevent libevent_core libevent_extras)

SET(PROTOBUF_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/third_part/protobuf" 
	"${CMAKE_CURRENT_SOURCE_DIR}/third_part/protobuf/google/protobuf")
SET(PROTOBUF_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/third_part/protobuf/lib")
SET(PROTOBUF_LIB libeay32 libprotobuf_d)

SET(MYSQL_INCLUDE "${CMAKE_CURRENT_SOURCE_DIR}/third_part/MySQL Connector.C 6.1/include")
SET(MYSQL_LIB_PATH "${CMAKE_CURRENT_SOURCE_DIR}/third_part/MySQL Connector.C 6.1/lib")
SET(MYSQL_LIB libmysql)

SET(BASE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/base")

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")

include(AddTarget)
include(AutoFiles)

#add_definitions(-D_UNICODE -D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS)
add_subdirectory(ClientServer)
add_subdirectory(AuthServer)
add_subdirectory(DatabaseServer)
add_subdirectory(DustbinServer)