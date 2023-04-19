
#include <stdio.h>
#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <vector>
#include <cmath>
#include <coroutine>
#include <filesystem>
#include <string>
#include "unique_generator.h"
#include "scripts.h"
#include "motion.h"

inline void changex(float value)
{
    current.pos.x += 1;
}