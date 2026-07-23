cmake_minimum_required(VERSION 3.28)

project(hmon LANGUAGES C CXX)

include(cmake/CompileOptions.cmake)
include(cmake/SanitizerOptions.cmake)
include(cmake/Output.cmake)

add_subdirectory(source)
add_subdirectory(third-party)
