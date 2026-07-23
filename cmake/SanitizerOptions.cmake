option(ALUSAN "{Address, Leak, Undefined} sanitizer" OFF)
option(MSAN "Memory sanitizer" OFF)
option(TSAN "Thread sanitizer" OFF)

if (ALUSAN)
    add_compile_options(-fsanitize=address,leak,undefined)
    add_link_options(-fsanitize=address,leak,undefined)
endif()

if (MSAN)
    add_compile_options(-fsanitize=memory)
    add_link_options(-fsanitize=memory)
endif()

if (TSAN)
    add_compile_options(-fsanitize=thread)
    add_link_options(-fsanitize=thread)
endif()

message(STATUS "ALUSAN: ${ALUSAN}")
message(STATUS "MSAN: ${MSAN}")
message(STATUS "TSAN: ${TSAN}")
