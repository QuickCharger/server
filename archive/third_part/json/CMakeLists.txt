CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)

SET(LIBNAME json)

SET(ENV)
IF(MSVC)
	IF(CMAKE_SIZEOF_VOID_P EQUAL 4)		# if(32bit)
		STRING(APPEND ENV "windows_x86_32")
		IF(MSVC_VERSION EQUAL 1900)		# if(vs2015)
			STRING(APPEND ENV "_msvc14")
		ENDIF()
	elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)	# if(64bit)
		STRING(APPEND ENV "windows_x86_64")
		IF(MSVC_VERSION EQUAL 1900)		# if(vs2015)
			STRING(APPEND ENV "_msvc14")
		ENDIF()
	ENDIF()
ENDIF()

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

INSTALL(TARGETS ${LIBNAME} CONFIGURATIONS Release  DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/${ENV}/Release)
INSTALL(TARGETS ${LIBNAME} CONFIGURATIONS Debug  DESTINATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/${ENV}/Debug)
