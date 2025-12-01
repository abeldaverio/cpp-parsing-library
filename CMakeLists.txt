cmake_minimum_required(VERSION 3.10)

project(parser)

set(CMAKE_CXX_COMPILER "/usr/bin/clang++")
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(LIB_NAME parser)

add_library(${LIB_NAME} STATIC
    src/BaseTypes/BaseTypes.cpp
    src/BaseTypes/Number.cpp
    src/Rest.cpp
    src/Utils.cpp
    src/JsonParser/JsonParser.cpp
    src/ByteParser/Utils.cpp
)

target_include_directories(${LIB_NAME}
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)
