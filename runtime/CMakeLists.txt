target_sources(app PRIVATE
  include_sdl.h
  sdl.h
  sdl.cpp
  sdl_scancodes.h

  value.h
  value.cpp

  coroutines.h

  ts_queue.h
  ts_container.h
  ts_synchronizer.h
  mutex.h

  control_flow.h

  utils.h

  time.h
  time.cpp

  files.h
  files.cpp

  array.h

  debug.h
  debug.cpp
)

add_subdirectory("math")
add_subdirectory("pen")
add_subdirectory("string")
add_subdirectory("script")
add_subdirectory("sprite")
add_subdirectory("sdl_gfx")
