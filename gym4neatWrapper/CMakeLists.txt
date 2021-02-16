cmake_minimum_required(VERSION 3.9)
project(gym4neat)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(STANDARD_UNIX_CXX_FLAGS "-W -Wall -Wextra -lcurl")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${STANDARD_UNIX_CXX_FLAGS}")

file(GLOB_RECURSE PROJECT_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.cpp" "include/*.hpp")

find_package(nlohmann_json 3.2.0 REQUIRED)

add_executable(gym4neatWrapper ${PROJECT_SOURCES})

target_include_directories(gym4neatWrapper PUBLIC "${PROJECT_SOURCE_DIR}/include/")

target_link_libraries(gym4neatWrapper PRIVATE nlohmann_json::nlohmann_json)