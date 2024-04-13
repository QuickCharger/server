CMAKE_MINIMUM_REQUIRED(VERSION 3.10 FATAL_ERROR)
project(Libevent)

set(LIBEVENT_VERSION "release-2.1.12-stable")
set(LIBEVENT_URL "https://github.com/libevent/libevent/archive/refs/tags/${LIBEVENT_VERSION}.tar.gz")
set(LIBEVENT_TAR_GZ "${CMAKE_CURRENT_BINARY_DIR}/libevent-${LIBEVENT_VERSION}.tar.gz")
set(LIBEVENT_SOURCE_DIR "${CMAKE_CURRENT_BINARY_DIR}/libevent-${LIBEVENT_VERSION}")
set(LIBEVENT_BUILD_DIR "${LIBEVENT_SOURCE_DIR}/build")

# 使用 curl 下载 libevent
find_program(CURL_EXECUTABLE NAMES curl PATHS C:/cygwin64/bin)
if(NOT EXISTS ${LIBEVENT_TAR_GZ})
  message(STATUS "Downloading libevent...")
  execute_process(
    COMMAND ${CURL_EXECUTABLE} -L ${LIBEVENT_URL} -o ${LIBEVENT_TAR_GZ}
    RESULT_VARIABLE CURL_RESULT
  )
  if(NOT CURL_RESULT EQUAL "0")
    message(FATAL_ERROR "Failed to download libevent from ${LIBEVENT_URL}")
  endif()
endif()

# 解压 libevent（如果还没有解压）
if(NOT EXISTS ${LIBEVENT_SOURCE_DIR})
  message(STATUS "Extracting libevent...")
  execute_process(
    COMMAND ${CMAKE_COMMAND} -E tar xzf ${LIBEVENT_TAR_GZ}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  )
endif()

# 配置 libevent 的 CMakeLists.txt（如果存在）
if(EXISTS "${LIBEVENT_SOURCE_DIR}/CMakeLists.txt")
  message(STATUS "Configuring and building libevent...")
  file(MAKE_DIRECTORY ${LIBEVENT_BUILD_DIR})
  execute_process(
    COMMAND ${CMAKE_COMMAND} -DEVENT__DISABLE_OPENSSL=ON -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/bin ..
    WORKING_DIRECTORY ${LIBEVENT_BUILD_DIR}
  )
  execute_process(
    COMMAND ${CMAKE_COMMAND} --build . --target install --clean-first --config Release
    WORKING_DIRECTORY ${LIBEVENT_BUILD_DIR}
  )
endif()