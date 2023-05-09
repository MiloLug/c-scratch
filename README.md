# C-Scratch Project

## Documentation

1. [Scratch blocks supported](docs/scratch_functionality.md)
2. [Project structure](docs/project_structure.md)
3. [About writing stage scripts etc.](docs/about_stage.md)
4. [Internals](docs/internals.md) (Not ready)

## Requirements

- ### Tools

  - CMake
  - make
  - GCC / Clang++

- ### Dependencies

  - SDL2
  - SDL2 Image
  - SDL2 gfx

## Getting Started

...

## Todo

- Get rid of SDL2 gfx
- Add atan2 from https://mazzo.li/posts/vectorized-atan2.html
- Add [`if on edge, bounce`], along with <`touching (edge) ?`>
- Implement variable input for <`touching (X) ?`> and (`distance to (X)`) and <`key (X) pressed?`>
- Implement [`contains`] for strings. Probably use https://github.com/FooBarWidget/boyer-moore-horspool
