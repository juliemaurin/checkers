cmake_minimum_required(VERSION 3.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

project(checkerspp)

file(GLOB SRC "*.h" "*.cpp")

add_executable(${PROJECT_NAME} ${SRC})
