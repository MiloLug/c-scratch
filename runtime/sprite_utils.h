#ifndef SPRITE_MANAGER_H
#define SPRITE_MANAGER_H

#include <vector>
#include "sdl.h"
#include "sprite.h"


typedef std::vector<Sprite *> SpritesList;

extern SpritesList spritesStorage;


void renderSprites(ScratchSDLWindow &window);
void initSprites(ScratchSDLWindow &window, const SpritesList &sprites);


#endif
