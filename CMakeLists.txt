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
  string(CONCAT COMMON_FLAGS
    "-fno-reorder-blocks "  # reordering blocks generates bad code sometimes
  )

  string(CONCAT COMMON_FLAGS_RELEASE
    "${COMMON_FLAGS}"
    "-ffast-math "
    "-march=native "
    "-fmodulo-sched "
    "-fgcse-sm "
    "-fgcse-las "
    "-fno-inline-small-functions "
  )

  if (MINGW)
    string(CONCAT COMMON_FLAGS_RELEASE "")
  endif()

  if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    string(CONCAT COMMON_FLAGS_RELEASE
      "${COMMON_FLAGS_RELEASE} "
      "-fcoroutines-ts "
      "-flto "
      "-fwhole-program-vtables "
      "-fforce-emit-vtables "
      "-faddrsig "
      "-Wno-ignored-optimization-argument "
      "-Wno-unused-command-line-argument "
    )

    set(CMAKE_CXX_FLAGS_DEBUG "-g ${COMMON_FLAGS} -save-temps")
    set(CMAKE_CXX_FLAGS_RELEASE "-Ofast ${COMMON_FLAGS_RELEASE}")

  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    string(CONCAT COMMON_FLAGS_RELEASE
      "${COMMON_FLAGS_RELEASE} "
      "-fcoroutines "
      "-fconcepts "
      "-flto=2 "
      "-fgcse-lm "
      "-fira-region=all "
    )

    set(CMAKE_CXX_FLAGS_DEBUG "-g ${COMMON_FLAGS} -save-temps")
    set(CMAKE_CXX_FLAGS_RELEASE "-Ofast ${COMMON_FLAGS_RELEASE}")

  endif()

endif()


# Executable settings
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
find_library(SDL2 SDL2_image SDL2_gfx REQUIRED)

add_executable(app
  config.h
  source.cpp
)

target_include_directories(app PRIVATE "${PROJECT_SOURCE_DIR}")
add_subdirectory("runtime")
add_subdirectory("stage")

if (MINGW)
  target_link_libraries(app PRIVATE mingw32)
endif()

target_link_libraries(app PRIVATE
  stdc++
  stdc++fs
  Threads::Threads
  m
  SDL2main
  SDL2
  SDL2_image
  SDL2_gfx
)

# Assets
file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
