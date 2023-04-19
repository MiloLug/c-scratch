struct Sprite
{
    SDL_FRect pos;
    float direction;
    int costumeNumber;
    float size;
    int visible;
    int layerOrder;
    std::vector<SDL_Texture*> textures;
};

unique_generator<void*> coro1();

unique_generator<void*> coro2();