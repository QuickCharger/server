CMAKE_MINIMUM_REQUIRED(VERSION 3.12)

SET(LIBNAME json)

SET(src
	include/json/allocator.h
	include/json/assertions.h
	include/json/autolink.h
	include/json/config.h
	include/json/features.h
	include/json/forwards.h
	include/json/json.h
	include/json/reader.h
	include/json/value.h
	include/json/version.h
	include/json/writer.h
	src/json_reader.cpp
	src/json_tool.h
	src/json_value.cpp
	src/json_valueiterator.inl
	src/json_writer.cpp
	src/version.h.in
	)

SET(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/lib)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/include)

ADD_LIBRARY(${LIBNAME} STATIC ${src})

INSTALL(TARGETS ${LIBNAME} DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib)
