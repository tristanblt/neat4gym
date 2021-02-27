cmake_minimum_required(VERSION 3.9.0)
project(gym4neat)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(STANDARD_UNIX_CXX_FLAGS "-W -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${STANDARD_UNIX_CXX_FLAGS}")
file(GLOB_RECURSE PROJECT_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "${PROJECT_SOURCE_DIR}/neatLib/src/*.cpp" "${PROJECT_SOURCE_DIR}/neatLib/include/*.hpp")

add_library(neatLib SHARED ${PROJECT_SOURCES})

set_target_properties(neatLib PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(neatLib PUBLIC "${PROJECT_SOURCE_DIR}/neatLib/include/")
