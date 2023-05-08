target_sources(app PRIVATE
  include_sdl.h
  sdl.h
  sdl.cpp

  sprite_base.h
  sprite.h
  backdrop.h
  sprite_manager.h
  sprite_manager.cpp

  string.h
  string_utils.h

  value.h
  value.cpp

  array.h

  math.h

  coroutines.h
  
  script_manager.h
  script_manager.cpp

  ts_queue.h
  ts_container.h
  ts_synchronizer.h
  mutex.h

  control_flow.h

  utils.h
  utils.cpp
)

add_subdirectory("math")
add_subdirectory("pen")
