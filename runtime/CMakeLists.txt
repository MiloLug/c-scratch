target_sources(app PRIVATE
  include_sdl.h
  sdl.h
  sdl.cpp
  sprite.h
  sprite_utils.h
  string.h
  value.h
  array.h
  math.h
  coroutines.h
  string_utils.h
  script_utils.h
  script_utils.cpp
  ts_queue.h
)

add_subdirectory("math")
