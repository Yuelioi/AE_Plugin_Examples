cmake_minimum_required(VERSION 3.0.0)
project(sdk_example VERSION 0.1.0)

include(CTest)
enable_testing()

add_library(sdk_example sdk_example.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
