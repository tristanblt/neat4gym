cmake_minimum_required(VERSION 3.9.0)
project(gym4neat C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(STANDARD_UNIX_CXX_FLAGS "-W -Wall -Wextra -lcurl")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${STANDARD_UNIX_CXX_FLAGS}")

file(GLOB_RECURSE PROJECT_SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "${PROJECT_SOURCE_DIR}/gym4neatWrapper/src/*.cpp" "${PROJECT_SOURCE_DIR}/gym4neatWrapper/include/*.hpp")

find_package(nlohmann_json 3.2.0 REQUIRED)
find_package(PythonLibs REQUIRED)

add_executable(gym4neatWrapper ${PROJECT_SOURCES})

target_include_directories(gym4neatWrapper PUBLIC "${PROJECT_SOURCE_DIR}/gym4neatWrapper/include/")
target_include_directories(gym4neatWrapper PUBLIC "${PROJECT_SOURCE_DIR}/../neatLib/include/")

target_link_libraries(gym4neatWrapper PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(gym4neatWrapper PRIVATE neatLib)
target_link_libraries(gym4neatWrapper PRIVATE ${PYTHON_LIBRARIES})

include_directories(${PYTHON_INCLUDE_DIRS})