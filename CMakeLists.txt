cmake_minimum_required(VERSION 3.0)
project(helens)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include (cmake/utils.cmake)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -fPIC -ggdb -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")
set(CMAKE_C_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -fPIC -ggdb -std=c11 -Wall -Wno-deprecated -Werror -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")

include_directories(.)
include_directories(/apps/helens/include)
link_directories(/apps/helens/lib)
link_directories(/apps/helens/lib64)

option(BUILD_TEST "ON for complile test" OFF)

find_package(Boost REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

set(LIB_SRC
    helens/config.cc
    helens/log.cc
    helens/mutex.cc
    helens/thread.cc
    helens/util.cc
    helens/fiber.cc
    )

ragelmaker(helens/http/http11_parser.rl LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/helens/http)
ragelmaker(helens/http/httpclient_parser.rl LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/helens/http)
ragelmaker(helens/uri.rl LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR}/helens)
protobufmaker(helens/ns/ns_protobuf.proto LIB_SRC ${CMAKE_CURRENT_SOURCE_DIR})

add_library(helens SHARED ${LIB_SRC})
force_redefine_file_macro_for_sources(helens)
find_library(PTHREAD pthread)

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
helens_add_executable(test1 "tests/test.cc" helens "${LIBS}")
helens_add_executable(test_config "tests/test_config.cc" helens "${LIBS}")
helens_add_executable(test_thread "tests/test_thread.cc" helens "${LIBS}")
helens_add_executable(test_thread "tests/test_fiber.cc" helens "${LIBS}")
set(ORM_SRCS
    helens/orm/table.cc
    helens/orm/column.cc
    helens/orm/index.cc
    helens/orm/orm.cc
    helens/orm/util.cc
    )
helens_add_executable(orm "${ORM_SRCS}" helens "${LIBS}")

add_library(test_module SHARED tests/test_module.cc)

set_target_properties(bin_helens PROPERTIES OUTPUT_NAME "helens")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
