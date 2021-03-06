INCLUDE_DIRECTORIES(
	${LIBEVENT_INCLUDE} 
	${PROTOBUF_INCLUDE}
	${GLOG_INCLUDE}
	${JSON_INCLUDE}
	${BASE_INCLUDE}

	${FRAME_INCLUDE}
	${PROJECT_PROTOBUF_INCLUDE}
)

LINK_DIRECTORIES(
	${LIBEVENT_LIB_PATH} 
	${PROTOBUF_LIB_PATH}
	${GLOG_LIB_PATH}
	${JSON_LIB_PATH}
	${BASE_LIB_PATH}
)

AUTO_FILES("." "src" "\\.(cc|cpp|h)$")
AUTO_FILES("../frame" "src" "\\.(cc|cpp|h|hpp)$")
AUTO_FILES("../NetProtocol" "src" "\\.(cc|cpp|h)$")

ADD_EXECUTABLE(ClientServer "${src}")

target_link_libraries(ClientServer ws2_32.lib wsock32.lib
	${LIBEVENT_LIB} 
	${PROTOBUF_LIB} 
	${GLOG_LIB}
	${JSON_LIB}
	${BASE_LIB}
)

SET_TARGET_PROPERTIES(ClientServer PROPERTIES 
	RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PROJECT_SOURCE_DIR}/bin
	RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PROJECT_SOURCE_DIR}/bin
)

STRING(REGEX REPLACE "/" "\\\\" FILE_SRC ${CMAKE_CURRENT_SOURCE_DIR}\\ClientServer.cfg)
STRING(REGEX REPLACE "/" "\\\\" FILE_DST ${PROJECT_SOURCE_DIR}\\bin)
ADD_CUSTOM_COMMAND(TARGET ClientServer POST_BUILD COMMAND copy /y ${FILE_SRC} ${FILE_DST})
