CMAKE_MINIMUM_REQUIRED(VERSION 3.10 FATAL_ERROR)

PROJECT(Server CXX)

# CMAKE_SOURCE_DIR 顶层cmake
# CMAKE_BINARY_DIR sln
# PROJECT_SOURCE_DIR Project下的顶层cmake

ADD_SUBDIRECTORY(third_part/libevent)

# LIBEVENT
SET(LIBEVENTBIN "${CMAKE_BINARY_DIR}/third_part/libevent/bin")
IF(WIN32)
	SET(LIBEVENT_INCLUDE	"${LIBEVENTBIN}/include")
	SET(LIBEVENT_LIB_PATH	"${LIBEVENTBIN}/lib")
	SET(LIBEVENT_LIB		"event" "event_core" "event_extra")
ELSEIF(UNIX AND NOT APPLE)
	SET(LIBEVENT_INCLUDE	"${LIBEVENTBIN}/include")
	SET(LIBEVENT_LIB_PATH	"${LIBEVENTBIN}/lib")
	SET(LIBEVENT_LIB		"event" "event_core" "event_extra" "event_pthreads")
ENDIF()

# EXTRA
IF(WIN32)
	SET(EXTRA_INCLUDE	"")
	SET(EXTRA_LIB_PATH	"")
	SET(EXTRA_LIB		"ws2_32.lib" "wsock32.lib")
ELSEIF(UNIX AND NOT APPLE)
	SET(EXTRA_INCLUDE	"")
	SET(EXTRA_LIB_PATH	"")
	SET(EXTRA_LIB		"pthread")
ENDIF()


IF(WIN32)
	add_definitions(-D_UNICODE -D_CRT_SECURE_NO_WARNINGS -D_SCL_SECURE_NO_WARNINGS -D_WINSOCK_DEPRECATED_NO_WARNINGS)
ENDIF()

INCLUDE_DIRECTORIES(
	${CMAKE_SOURCE_DIR}/src
	${LIBEVENT_INCLUDE} 
)

LINK_DIRECTORIES(
	${LIBEVENT_LIB_PATH} 
)

file(GLOB_RECURSE SOURCES
	"${CMAKE_SOURCE_DIR}/src/*.h"
	"${CMAKE_SOURCE_DIR}/src/*.cpp"
	"${CMAKE_SOURCE_DIR}/src/*.cc"
)

ADD_EXECUTABLE(server ${SOURCES})

target_link_libraries(server
	${LIBEVENT_LIB}
	${EXTRA_LIB}
)

SET_TARGET_PROPERTIES(server PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin
	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin
	RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin
)

# 在build完成后
# 复制必要的动态库
IF(WIN32)
    add_custom_command(TARGET server POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${LIBEVENTBIN}/lib/event.dll" "${PROJECT_SOURCE_DIR}/bin/"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${LIBEVENTBIN}/lib/event_core.dll" "${PROJECT_SOURCE_DIR}/bin/"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${LIBEVENTBIN}/lib/event_extra.dll" "${PROJECT_SOURCE_DIR}/bin/"
    )
ELSEIF(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    add_custom_command(TARGET server POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${LIBEVENTBIN}/lib/libevent-2.1.so.7.0.1" "${PROJECT_SOURCE_DIR}/bin/libevent-2.1.so"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${LIBEVENTBIN}/lib/libevent_core-2.1.so.7.0.1" "${PROJECT_SOURCE_DIR}/bin/libevent_core-2.1.so"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${LIBEVENTBIN}/lib/libevent_extra-2.1.so.7.0.1" "${PROJECT_SOURCE_DIR}/bin/libevent_extra-2.1.so"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "${LIBEVENTBIN}/lib/libevent_pthreads-2.1.so.7.0.1" "${PROJECT_SOURCE_DIR}/bin/libevent_pthreads-2.1.so.7.0.1"
    )
ENDIF()
