target_sources(app PRIVATE
  include_sdl.h
  sdl.h
  sdl.cpp
  sdl_scancodes.h

  sprite_base.h
  sprite.h
  backdrop.h
  sprite_manager.h
  sprite_manager.cpp

  string.h

  value.h
  value.cpp

  array.h

  coroutines.h
  
  script_manager.h
  script_manager.cpp

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
)

add_subdirectory("math")
add_subdirectory("pen")
add_subdirectory("string_utils")
