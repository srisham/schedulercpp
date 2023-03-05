cmake_minimum_required(VERSION 3.0.0)
project(schedulercpp VERSION 1.0.0)

SET(GCC_COVERAGE_COMPILE_FLAGS "-Werror -Wall -std=c++17")
SET(CMAKE_CXX_FLAGS  "${GCC_COVERAGE_COMPILE_FLAGS}")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH})

add_executable(${PROJECT_NAME} 
    ${CMAKE_SOURCE_DIR}/src/jobscheduler.cpp
    ${CMAKE_SOURCE_DIR}/src/main.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)