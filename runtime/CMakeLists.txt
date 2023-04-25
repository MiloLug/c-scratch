target_sources(app PRIVATE
  include_sdl.h
  sdl.h
  sdl.cpp

  sprite.h
  sprite_utils.h

  string.h
  string.cpp
  string_utils.h

  value.h
  value.cpp

  array.h

  math.h

  coroutines.h
  
  script_utils.h
  script_utils.cpp

  ts_queue.h
)

add_subdirectory("math")
