target_sources(app PRIVATE
  include_sdl.h
  sdl.h
  sdl.cpp
  sprite.h
  sprite_manager.h
  string.h
  value.h
  array.h
  math.h
  coroutines.h
  string_utils.h
)

add_subdirectory("math")
