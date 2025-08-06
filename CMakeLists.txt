cmake_minimum_required(VERSION 3.10 FATAL_ERROR)
# We follow a "build" versioning system in which the build number increases with each new release.
project("Rogue" LANGUAGES C VERSION 1)


include(FetchContent)
FetchContent_Declare(
    "Waterlily"
    GIT_REPOSITORY "https://github.com/israfiel-a/waterlily.git"
    GIT_TAG "origin/main"
)
FetchContent_MakeAvailable("Waterlily")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

add_executable("Rogue" "${CMAKE_CURRENT_SOURCE_DIR}/Source/Entry.c")
add_dependencies("Rogue" "Waterlily")
target_link_libraries("Rogue" "Waterlily")

file(COPY "${CMAKE_CURRENT_SOURCE_DIR}/Assets" DESTINATION "${CMAKE_CURRENT_BINARY_DIR}")
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Assets/Shaders/Compiled")
