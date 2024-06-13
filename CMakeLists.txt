project(cmake_test_project)
cmake_minimum_required(VERSION 3.16)

include_directories(${PROJECT_SOURCE_DIR}/src)

message(STATUS "!!!!!!!!!! ${PROJECT_SOURCE_DIR}")
add_subdirectory(${PROJECT_SOURCE_DIR}/src)