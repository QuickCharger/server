CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)
PROJECT(Server CXX)

SET(THIRDPATH ${CMAKE_SOURCE_DIR}/third_part)

SET(ENV)
IF(MSVC)
	IF(CMAKE_SIZEOF_VOID_P EQUAL 4)			# if(32bit)
		STRING(APPEND ENV "windows_x86_32")
		IF(MSVC_VERSION EQUAL 1900)		# if(vs2015)
			STRING(APPEND ENV "_msvc14")
		ENDIF()
	elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)		# if(64bit)
		STRING(APPEND ENV "windows_x86_64")
		IF(MSVC_VERSION EQUAL 1900)		# if(vs2015)
			STRING(APPEND ENV "_msvc14")
		ENDIF()
	ENDIF()
ENDIF()

MESSAGE("ENV " ${ENV})

# LIBEVENT
SET(LIBEVENT_INCLUDE	"${THIRDPATH}/libevent-2.1.8-stable/include" "${THIRDPATH}/libevent-2.1.8-stable/WIN32-Code/nmake")
SET(LIBEVENT_LIB_PATH	"${THIRDPATH}/libevent-2.1.8-stable/lib/${ENV}")
SET(LIBEVENT_LIB		libevent libevent_core libevent_extras)

# PROTOBUF
SET(PROTOBUF_INCLUDE	"${THIRDPATH}/protobuf-2.6.1/include")
SET(PROTOBUF_LIB_PATH	"${THIRDPATH}/protobuf-2.6.1/lib/${ENV}")
SET(PROTOBUF_LIB libprotobuf)

# MYSQL
SET(MYSQL_INCLUDE	"${THIRDPATH}/MySQL Connector.C 6.1/include")
SET(MYSQL_LIB_PATH	"${THIRDPATH}/MySQL Connector.C 6.1/lib")
SET(MYSQL_LIB libmysql)

# GLOG
SET(GLOG_INCLUDE		"${THIRDPATH}/glog-0.3.5/src/windows")
SET(GLOG_LIB_PATH		"${THIRDPATH}/glog-0.3.5/lib/${ENV}")
SET(GLOG_LIB libglog_static)

# JSON
SET(JSON_INCLUDE		"${THIRDPATH}/json/include")
SET(JSON_LIB_PATH		"${THIRDPATH}/json/lib/${ENV}")
SET(JSON_LIB			json)

# BASE
SET(BASE_INCLUDE		"${THIRDPATH}/base/include")
SET(BASE_LIB_PATH		"${THIRDPATH}/base/lib/${ENV}")
SET(BASE_LIB			base)

# IMPORT FUNCTION
SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${PROJECT_SOURCE_DIR}/cmake")
include(AddTarget)
include(AutoFiles)

IF(WIN32)
	add_definitions(-D_UNICODE -D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS -D_WINSOCK_DEPRECATED_NO_WARNINGS)
ENDIF()

add_subdirectory(third_part/base)

add_subdirectory(server)