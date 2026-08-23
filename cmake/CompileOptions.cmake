# Compile Options for project

# By Default
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_compile_options(-Wall -Wextra -Wpedantic)

message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")

# Other...
option(LTO "Link Time Optimization (LTO)" OFF)

if (LTO)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT lto_supported OUTPUT lto_error)
    if(lto_supported)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
        message(STATUS "LTO: ON")
    else()
        message(FATAL_ERROR "LTO not supported: ${lto_error}")
    endif()
endif()
