cmake_minimum_required(VERSION 3.16.3)

project(c_scratch_project LANGUAGES CXX)

# Compilation setings
set (CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Executable settings
find_library(SDL2 SDL2_image)

add_executable(app
  config.h
  scripts.h
  source.cpp
)

target_include_directories(app PRIVATE "${PROJECT_SOURCE_DIR}")
add_subdirectory("runtime")

target_link_libraries(app SDL2 SDL2main SDL2_image)

# Assets
file(COPY sprites DESTINATION ${CMAKE_BINARY_DIR})
