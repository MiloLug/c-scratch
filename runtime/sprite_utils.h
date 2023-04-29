#ifndef SPRITE_MANAGER_H
#define SPRITE_MANAGER_H

#include <vector>
#include "include_sdl.h"
#include "sprite.h"


typedef std::vector<Sprite *> SpritesList;

extern SpritesList spritesStorage;


void renderSprites(SDL_Renderer *renderer);
void initSprites(SDL_Renderer *renderer, const SpritesList &sprites);


#endif
