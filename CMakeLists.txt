cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
project("Example" LANGUAGES C VERSION 0.0.0.11)

add_executable(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/Example.c")

add_dependencies(Example Waterlily)
target_link_libraries(Example Waterlily)

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/Assets" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Assets/Shaders/Compiled")