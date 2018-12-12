CMAKE_MINIMUM_REQUIRED(VERSION 3.12)

SET(LIBNAME base)

SET(src
	include/CBuffer.h	src/CBuffer.cpp
	include/CBufferRecv.h	src/CBufferRecv.cpp
	include/CBufferSend.h	src/CBufferSend.cpp
	include/CErrRecord.h
	include/CFile.h		src/CFile.cpp
	include/CTime.h		src/CTime.cpp
	include/Func.h		src/Func.cpp
	include/macro.h)

SET(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/lib)

ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)

ADD_LIBRARY(${LIBNAME} STATIC ${src})

#SET_TARGET_PROPERTIES(${LIBNAME} PROPERTIES DEBUG_POSTFIX "_d")

INSTALL(TARGETS ${LIBNAME} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib)
