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

## Compilation

Clone the repo with `git clone https://github.com/MiloLug/c-scratch.git`.

### Windows
...

### Linux
...

### Mac OS

1. Install homebrew: https://brew.sh/ (don't forget to reboot).
2. Now install SDL2 and SD2 Image with `brew install sdl2 sd2_image`.
3. Check your clang version: `clang --version`. If it's below 16, install llvm and it's toolchain with `brew install llvm`.
4. Now you can go to the project directory in the terminal: `cd c-scratch`.

#### If you want just cmake and make (no xcode)
1. If you've installed Cmake with brew, you have to configure the compiler path for the project when running cmake:
   ```shell
   cmake -DCMAKE_C_COMPILER=/opt/homebrew/opt/llvm/bin/clang -DCMAKE_CXX_COMPILER=/opt/homebrew/opt/llvm/bin/clang++ .
   ```
  
   Otherwise it's just `cmake .`
2. Run `make`
3. Enjoy your `app` executable

#### If you want Xcode for whatever reason
1. To generate the project, use `cmake -G Xcode -B xcode-base .` - this command will generate the project inside xcode-base directory.
2. Open `xcode-base/<project name>.xcodeproj` file with Xcode.
3. IF CLANG VERSION < 16
   
   Now you have to set a couple of custom flags in the project settings:
   - `CC` = `/opt/homebrew/opt/llvm/bin/clang`
   - `CXX` = `/opt/homebrew/opt/llvm/bin/clang++`
   - `COMPILER_INDEX_STORE_ENABLE` = `NO`
  
   For this, click at you project name in the project (files) navigator, then go to "Build settings" tab.

   Then click the plus button (it's on the line with search field and buttons like "Basic", "Customized", etc.) and select "Add user-defined setting".
6. Now you can work with this as any othe Xcode project.

But I still recommend using VSCode for development, especially for custom Clang setup (there are many guides out there), or try CLion.

## Todo and ideas

- Get rid of SDL2 gfx
- Add atan2 from https://mazzo.li/posts/vectorized-atan2.html
- Add [`if on edge, bounce`], along with <`touching (edge) ?`>
- Implement variable input for <`touching (X) ?`> and (`distance to (X)`)
- Add some compilation scripts
- Make some stuff to compile something like packages with the stage code automatically...
