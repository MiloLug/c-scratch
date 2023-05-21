cmake_minimum_required(VERSION 3.16.3)

project(c_scratch_project LANGUAGES C CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

# Compilers setings and flags

set (CMAKE_CXX_STANDARD 20)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

set(COMPILER "")
set(COMPILER_ENV "")


if (CMAKE_GENERATOR MATCHES "Visual Studio" OR CMAKE_CXX_SIMULATE_ID MATCHES "MSVC" OR CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
  set(COMPILER_ENV "VS")
else()
  set(COMPILER_ENV "UNKNOWN")
endif()

if (COMPILER_ENV STREQUAL "VS")
  string(CONCAT COMMON_FLAGS
    "/fp:fast "
    "/await "
    "/MP "  # Use multiprocessing for compilation
    "/volatile:iso "
  )

  string(CONCAT DEBUG_FLAGS
    "/DEBUG "
  )

  string(CONCAT RELEASE_FLAGS
    "/Oi "
    "/Ot "
    "/Oy "
    "/O2 "
    "/Ob1 "

    "/Gr "  # __fastcall
    "/GF "  # Eliminate Duplicate Strings
    "/GL "  # Whole program optimization
    "/Gw "  # Optimize Global Data
    "/GA "  # Optimize for Windows Application
    "/Gy "  # Enable Function-Level Linking
    "/favor:INTEL64 "
  )

  if (HAVE_AVX2_EXTENSIONS)
      set(COMMON_FLAGS "${COMMON_FLAGS} /arch:AVX2 ")
  elseif (HAVE_AVX_EXTENSIONS)
      set(COMMON_FLAGS "${COMMON_FLAGS} /arch:AVX ")
  endif()

  set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS_RELEASE} /LTCG")

  if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    message("Using Cmake-cl compiler")

    set(COMPILER "CLANG")
  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    message("Using MSVC compiler")
  
    set(COMPILER "MSVC")
  endif()

else()
  string(CONCAT COMMON_FLAGS
    "-fno-reorder-blocks "  # reordering blocks generates bad code sometimes
    "-ffast-math "
    "-march=native "
  )

  string(CONCAT DEBUG_FLAGS
    "-g "
    "-save-temps "
  )

  string(CONCAT RELEASE_FLAGS
    "-Ofast "
    "-fmodulo-sched "
    "-fgcse-sm "
    "-fgcse-las "
    "-fno-inline-small-functions "
  )

  if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    message("Using Cmake compiler")

    set(COMPILER "CLANG")
    
    if (CMAKE_CXX_COMPILER_VERSION VERSION_LESS 16)
      string(CONCAT COMMON_FLAGS "${COMMON_FLAGS} -fcoroutines-ts ")
    endif()

    string(CONCAT COMMON_FLAGS
      "${COMMON_FLAGS}"
      "-Wno-ignored-optimization-argument "
      "-Wno-unused-command-line-argument "
    )

    string(CONCAT RELEASE_FLAGS
      "${RELEASE_FLAGS}"
      "-flto "
      "-fwhole-program-vtables "
      "-fforce-emit-vtables "
      "-faddrsig "
    )
  elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    message("Using GNU compiler")

    set(COMPILER "GCC")

    string(CONCAT COMMON_FLAGS
      "${COMMON_FLAGS} "
      "-fcoroutines "
      "-fconcepts "
    )

    string(CONCAT RELEASE_FLAGS
      "${RELEASE_FLAGS} "
      "-flto=2 "
      "-fgcse-lm "
      "-fira-region=all "
    )
  endif()

endif()

set(CMAKE_CXX_FLAGS_DEBUG "${COMMON_FLAGS} ${DEBUG_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "${COMMON_FLAGS} ${RELEASE_FLAGS}")
set(CMAKE_C_FLAGS_DEBUG "${COMMON_FLAGS} ${DEBUG_FLAGS}")
set(CMAKE_C_FLAGS_RELEASE "${COMMON_FLAGS} ${RELEASE_FLAGS}")

# Executable settings

set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)

if (COMPILER_ENV STREQUAL "VS")
  set(CMAKE_PREFIX_PATH libs/msvc)
endif()
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

add_executable(app
  config.h
  source.cpp
)

target_include_directories(app PRIVATE "${PROJECT_SOURCE_DIR}")
add_subdirectory("runtime")
add_subdirectory("stage")

if (COMPILER STREQUAL "GCC")
  target_link_libraries(app stdc++ stdc++fs)
endif()

if (NOT COMPILER_ENV STREQUAL "VS")
  target_link_libraries(app m)
endif()

target_link_libraries(app
  Threads::Threads
  SDL2::Main
  SDL2::Core
  SDL2::Image
)

# Some variables for the code

target_compile_definitions(app PRIVATE COMPILER_${COMPILER} COMPILER_ENV_${COMPILER_ENV})

# Assets

file(COPY assets DESTINATION ${CMAKE_BINARY_DIR})
