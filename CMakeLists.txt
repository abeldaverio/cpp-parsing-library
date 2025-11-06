cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 23)
set(LIB_NAME parser)

add_library(${LIB_NAME} STATIC
    src/BaseTypes.cpp
    src/Rest.cpp
)

target_include_directories(${LIB_NAME}
    PRIVATE
        ${CMAKE_SOURCE_DIR}/include
)
