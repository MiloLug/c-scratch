target_sources(app PRIVATE
  common.h
  rand.h
  sign.h
  math.h
  exp.h
)

add_subdirectory("fast")
add_subdirectory("precise")
