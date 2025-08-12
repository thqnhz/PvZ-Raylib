cmake_minimum_required(VERSION 3.15)
project(PvZ LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# If raylib is already installed on the system:
find_package(raylib 5.0 QUIET)

# If not found, use submodule
if (NOT raylib_FOUND)
    message(STATUS "raylib not found, using submodule...")
    add_subdirectory(include/raylib)
endif()

# Game executable
file(GLOB_RECURSE GAME_SOURCES CONFIGURE_DEPENDS src/*.cpp)
add_executable(${PROJECT_NAME} ${GAME_SOURCES})

target_include_directories(${PROJECT_NAME} PRIVATE src)

target_link_libraries(${PROJECT_NAME} raylib)

# Copy assets to build directory
file(COPY asset DESTINATION ${CMAKE_BINARY_DIR})
