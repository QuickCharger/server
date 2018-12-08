INCLUDE_DIRECTORIES(${LIBEVENT_INCLUDE} 
	${PROTOBUF_INCLUDE}
	${GLOG_INCLUDE}
	${BASE_PATH}
	${BASE_FRAME}
	${MYSQL_INCLUDE}
	${BASE_PROTO_SERVER}
)
LINK_DIRECTORIES(${LIBEVENT_LIB_PATH} 
	${PROTOBUF_LIB_PATH} 
	${MYSQL_LIB_PATH}
	${GLOG_LIB_PATH}
)

AUTO_FILES("." "src" "\\.(cc|cpp|h)$")
AUTO_FILES("../base" "src" "\\.(cc|cpp|h)$")
AUTO_FILES("../frame" "src" "\\.(cc|cpp|h)$")
AUTO_FILES("../NetProtocol" "src" "\\.(cc|cpp|h)$")
#AUTO_FILES("../NetProtocol/server" "src" "\\.(cc|cpp|h)$")
#AUTO_FILES("../NetProtocol/common" "src" "\\.(cc|cpp|h)$")

ADD_EXECUTABLE(DatabaseServer "${src}")
target_link_libraries(DatabaseServer ws2_32.lib wsock32.lib 
	${LIBEVENT_LIB} 
	${PROTOBUF_LIB} 
	${MYSQL_LIB}
	${GLOG_LIB}
)