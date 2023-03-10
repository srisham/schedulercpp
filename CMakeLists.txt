cmake_minimum_required(VERSION 3.0.0)
project(schedulercpp VERSION 1.0.1)

SET(GCC_COVERAGE_COMPILE_FLAGS "-Werror -Wall -std=c++17")
SET(CMAKE_CXX_FLAGS  "${GCC_COVERAGE_COMPILE_FLAGS}")

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH})

add_library(${PROJECT_NAME} SHARED
    ${CMAKE_SOURCE_DIR}/src/jobscheduler.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

set_target_properties(${PROJECT_NAME}  PROPERTIES VERSION ${PROJECT_VERSION})

set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION 1)

set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER include/jobscheduler.h)

install(TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)