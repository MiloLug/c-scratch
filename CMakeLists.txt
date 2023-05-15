cmake_minimum_required(VERSION 3.16.3)

project(c_scratch_project LANGUAGES C CXX)

# Compilation setings
set (CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

if (CMAKE_GENERATOR MATCHES "Visual Studio" OR MSVC OR CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  set(AVX_FLAGS)
  if(HAVE_AVX2_EXTENSIONS AND NOT MSVC_VERSION LESS 1800)
      set(AVX_FLAGS "${AVX_FLAGS} /arch:AVX2")
  elseif(HAVE_AVX_EXTENSIONS  AND NOT MSVC_VERSION LESS 1600)
      set(AVX_FLAGS "${AVX_FLAGS} /arch:AVX")
  endif()

  set(CMAKE_CXX_FLAGS_DEBUG "/fp:fast /await ${AVX_FLAGS}")
  set(CMAKE_CXX_FLAGS_RELEASE "/fp:fast /await ${AVX_FLAGS}")

else()
  set(COMMON_FLAGS "-ffast-math -march=native")

  if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(COMMON_FLAGS "${COMMON_FLAGS} -fcoroutines-ts -flto")

    set(CMAKE_CXX_FLAGS_DEBUG "-g ${COMMON_FLAGS} -save-temps")
    set(CMAKE_CXX_FLAGS_RELEASE "-Ofast ${COMMON_FLAGS}")

  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(COMMON_FLAGS "${COMMON_FLAGS} -fcoroutines -fconcepts -flto=2")

    set(CMAKE_CXX_FLAGS_DEBUG "-g ${COMMON_FLAGS} -save-temps")
    set(CMAKE_CXX_FLAGS_RELEASE "-Ofast ${COMMON_FLAGS}")

  endif()

endif()


# Executable settings
find_library(SDL2 SDL2_image SDL2_gfx)

add_executable(app
  config.h
  source.cpp
)

target_include_directories(app PRIVATE "${PROJECT_SOURCE_DIR}")
add_subdirectory("runtime")
add_subdirectory("stage")

target_link_libraries(app SDL2 SDL2main SDL2_image SDL2_gfx)

# Assets
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
