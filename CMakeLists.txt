cmake_minimum_required(VERSION 3.21)

project(gdbot VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT DEFINED GEODE_SDK)
    set(GEODE_SDK "$ENV{GEODE_SDK}")
endif()

if(NOT GEODE_SDK)
    set(GEODE_SDK "${CMAKE_SOURCE_DIR}/geode-sdk")
endif()

if(NOT EXISTS "${GEODE_SDK}/CMakeLists.txt")
    message(FATAL_ERROR
        "Geode SDK not found. Clone it into ${GEODE_SDK} or set GEODE_SDK to the SDK directory."
    )
endif()

add_subdirectory("${GEODE_SDK}" "${CMAKE_BINARY_DIR}/geode-sdk")

add_library(gdbot SHARED
    src/main.cpp
)

setup_geode_mod(gdbot)
