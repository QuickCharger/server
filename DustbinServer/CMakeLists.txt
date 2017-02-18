INCLUDE_DIRECTORIES(${LIBEVENT_INCLUDE} 
	${PROTOBUF_INCLUDE}
	${BASE_PATH}
)
LINK_DIRECTORIES(${LIBEVENT_LIB_PATH} 
	${PROTOBUF_LIB_PATH} 
)

AUTO_FILES("." "src" "\\.(cc|cpp|h)$")
AUTO_FILES("../base" "src" "\\.(cc|cpp|h)$")

ADD_EXECUTABLE(DustbinServer "${src}")
target_link_libraries(DustbinServer ws2_32.lib wsock32.lib 
	${LIBEVENT_LIB} 
	${PROTOBUF_LIB} 
)