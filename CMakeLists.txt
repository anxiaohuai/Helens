cmake_minimum_required(VERSION 3.0)
project(helens)

#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdpart/yaml-cpp)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdpart/jsoncpp-1.8.4)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdpart/sqlite-3.28.0)
#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/thirdpart/tinyxml2-7.0.1)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include (cmake/utils.cmake)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -fPIC -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")
set(CMAKE_C_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -fPIC -ggdb -std=c11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")

include_directories(.)
include_directories(/apps/sylar/include)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdpart)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdpart/yaml-cpp/include)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdpart/jsoncpp-1.8.4/include)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdpart/sqlite-3.28.0)
#include_directories(${CMAKE_CURRENT_SOURCE_DIR}/thirdpart/tinyxml2-7.0.1)
link_directories(/apps/sylar/lib)
link_directories(/apps/sylar/lib64)

option(BUILD_TEST "ON for complile test" OFF)

find_package(Boost REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

find_package(MySQL REQUIRED)
if(MYSQL_FOUND)
    include_directories(${MYSQL_INCLUDE_DIR})
endif()

find_package(Protobuf)
if(Protobuf_FOUND)
    include_directories(${Protobuf_INCLUDE_DIRS})
endif()
find_package(OpenSSL REQUIRED)
if(OPENSSL_FOUND)
    include_directories(${OPENSSL_INCLUDE_DIR})
endif()

find_package(ZLIB REQUIRED)
if(ZLIB_FOUND)
    include_directories(${ZLIB_INCLUDE_DIR})
endif()

set(LIB_SRC
    helens/address.cc
    helens/bytearray.cc
    helens/config.cc
    helens/db/fox_thread.cc
    helens/db/mysql.cc
    helens/db/redis.cc
    helens/db/sqlite3.cc
    helens/ds/bitmap.cc
    helens/ds/roaring_bitmap.cc
    helens/ds/roaring.c
    helens/ds/util.cc
    helens/email/email.cc
    helens/email/smtp.cc
    helens/env.cc
    helens/daemon.cc
    helens/fd_manager.cc
    helens/fiber.cc
    helens/http/http.cc
    helens/http/http_connection.cc
    helens/http/http_parser.cc
    helens/http/http_session.cc
    helens/http/http_server.cc
    helens/http/servlet.cc
    helens/http/servlets/config_servlet.cc
    helens/http/servlets/status_servlet.cc
    helens/http/session_data.cc
    helens/http/ws_connection.cc
    helens/http/ws_session.cc
    helens/http/ws_server.cc
    helens/http/ws_servlet.cc
    helens/hook.cc
    helens/iomanager.cc
    helens/library.cc
    helens/log.cc
    helens/module.cc
    helens/mutex.cc
    helens/ns/name_server_module.cc
    helens/ns/ns_client.cc
    helens/ns/ns_protocol.cc
    helens/protocol.cc
    helens/rock/rock_protocol.cc
    helens/rock/rock_server.cc
    helens/rock/rock_stream.cc
    helens/scheduler.cc
    helens/socket.cc
    helens/stream.cc
    helens/streams/async_socket_stream.cc
    helens/streams/socket_stream.cc
    helens/streams/load_balance.cc
    helens/streams/service_discovery.cc
    helens/streams/zlib_stream.cc
    helens/tcp_server.cc
    helens/timer.cc
    helens/thread.cc
    helens/util.cc
    helens/util/crypto_util.cc
    helens/util/json_util.cc
    helens/util/hash_util.cc
    helens/worker.cc
    helens/application.cc
    helens/zk_client.cc
    )

ragelmaker(helens/http/http11_parser.rl LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/helens/http)
ragelmaker(helens/http/httpclient_parser.rl LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/helens/http)
ragelmaker(helens/uri.rl LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/helens)

protobufmaker(helens/ns/ns_protobuf.proto LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR})
#PROTOBUF_GENERATE_CPP(PB_SRCS PB_HDRS helens/ns/ns_protobuf.proto)
#message("****${PB_SRCS}***${PB_HDRS}****")
#list(APPEND LIB_SRC ${PB_SRCS})
#
#message(STATUS ${LIB_SRC})

add_library(helens SHARED ${LIB_SRC})
force_redefine_file_macro_for_sources(helens)
#add_library(helens_static STATIC ${LIB_SRC})
#SET_TARGET_PROPERTIES (helens_static PROPERTIES OUTPUT_NAME "helens")

#find_library(YAMLCPP yaml-cpp)
find_library(PTHREAD pthread)
#set(LIBS ${LIBS} helens)
#set(LIBS ${LIBS} dl)
#set(LIBS ${LIBS} ${YAMLCPP})
#set(LIBS ${LIBS} ${PTHREAD})

set(LIBS
        helens
        dl
        pthread
        yaml-cpp
        jsoncpp
        ${ZLIB_LIBRARIES}
        ${OPENSSL_LIBRARIES}
        ${PROTOBUF_LIBRARIES}
        event
        hiredis_vip
        mysqlclient_r
        zookeeper_mt
        sqlite3
        tinyxml2
        jemalloc
        )
helens_add_executable(test_util "tests/test_util.cc" helens "${LIBS}")
helens_add_executable(test_hashmultimap "tests/test_hashmultimap.cc" helens "${LIBS}")
helens_add_executable(test_hashmap "tests/test_hashmap.cc" helens "${LIBS}")
helens_add_executable(test_dict "tests/test_dict.cc" helens "${LIBS}")
helens_add_executable(test_array "tests/test_array.cc" helens "${LIBS}")
#if(BUILD_TEST)
helens_add_executable(test1 "tests/test.cc" helens "${LIBS}")
helens_add_executable(test_config "tests/test_config.cc" helens "${LIBS}")
helens_add_executable(test_thread "tests/test_thread.cc" helens "${LIBS}")
helens_add_executable(test_fiber "tests/test_fiber.cc" helens "${LIBS}")
helens_add_executable(test_scheduler "tests/test_scheduler.cc" helens "${LIBS}")
helens_add_executable(test_iomanager "tests/test_iomanager.cc" helens "${LIBS}")
helens_add_executable(test_hook "tests/test_hook.cc" helens "${LIBS}")
helens_add_executable(test_address "tests/test_address.cc" helens "${LIBS}")
helens_add_executable(test_socket "tests/test_socket.cc" helens "${LIBS}")
helens_add_executable(test_bytearray "tests/test_bytearray.cc" helens "${LIBS}")
helens_add_executable(test_http "tests/test_http.cc" helens "${LIBS}")
helens_add_executable(test_http_parser "tests/test_http_parser.cc" helens "${LIBS}")
helens_add_executable(test_tcp_server "tests/test_tcp_server.cc" helens "${LIBS}")
helens_add_executable(echo_server "examples/echo_server.cc" helens "${LIBS}")
helens_add_executable(test_http_server "tests/test_http_server.cc" helens "${LIBS}")
helens_add_executable(test_uri "tests/test_uri.cc" helens "${LIBS}")
helens_add_executable(my_http_server "samples/my_http_server.cc" helens "${LIBS}")

helens_add_executable(echo_server_udp "examples/echo_server_udp.cc" helens "${LIBS}")
helens_add_executable(echo_udp_client "examples/echo_udp_client.cc" helens "${LIBS}")

helens_add_executable(test_daemon "tests/test_daemon.cc" helens "${LIBS}")
helens_add_executable(test_env "tests/test_env.cc" helens "${LIBS}")
helens_add_executable(test_ws_server "tests/test_ws_server.cc" helens "${LIBS}")
helens_add_executable(test_ws_client "tests/test_ws_client.cc" helens "${LIBS}")
helens_add_executable(test_application "tests/test_application.cc" helens "${LIBS}")

helens_add_executable(test_http_connection "tests/test_http_connection.cc" helens "${LIBS}")
helens_add_executable(test_lru "tests/test_lru.cc" helens "${LIBS}")
helens_add_executable(test_timed_cache "tests/test_timed_cache.cc" helens "${LIBS}")
helens_add_executable(test_timed_lru_cache "tests/test_timed_lru_cache.cc" helens "${LIBS}")
helens_add_executable(test_zlib_stream "tests/test_zlib_stream.cc" helens "${LIBS}")

#endif()
helens_add_executable(test_crypto "tests/test_crypto.cc" helens "${LIBS}")
helens_add_executable(test_sqlite3 "tests/test_sqlite3.cc" helens "${LIBS}")
helens_add_executable(test_rock "tests/test_rock.cc" helens "${LIBS}")
helens_add_executable(test_email  "tests/test_email.cc" helens "${LIBS}")
helens_add_executable(test_mysql "tests/test_mysql.cc" helens "${LIBS}")
helens_add_executable(test_nameserver "tests/test_nameserver.cc" helens "${LIBS}")
helens_add_executable(test_bitmap "tests/test_bitmap.cc" helens "${LIBS}")
helens_add_executable(test_zkclient "tests/test_zookeeper.cc" helens "${LIBS}")
helens_add_executable(test_service_discovery "tests/test_service_discovery.cc" helens "${LIBS}")

set(ORM_SRCS
    helens/orm/table.cc
    helens/orm/column.cc
    helens/orm/index.cc
    helens/orm/orm.cc
    helens/orm/util.cc
    )
helens_add_executable(orm "${ORM_SRCS}" helens "${LIBS}")

add_library(test_module SHARED tests/test_module.cc)

helens_add_executable(bin_helens "helens/main.cc" helens "${LIBS}")
set_target_properties(bin_helens PROPERTIES OUTPUT_NAME "helens")

#add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/orm_out)
#set(OLIBS ${LIBS} orm_data)
#helens_add_executable(test_orm "tests/test_orm.cc" orm_data "${OLIBS}")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
