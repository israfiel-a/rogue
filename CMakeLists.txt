cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project("Rogue" LANGUAGES C VERSION 0.0.1)

add_executable("Rogue" "${CMAKE_CURRENT_SOURCE_DIR}/Source/Entry.c")

add_dependencies("Rogue" Waterlily)
target_link_libraries("Rogue" Waterlily)

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/Assets" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Assets/Shaders/Compiled")
