#include "TileSet.h"

TileSet::TileSet()
{
}

TileSet::~TileSet()
{
    for (Texture2D& texture : textures) 
    {
        UnloadTexture(texture);
    }
}

void TileSet::init()
{
    // tile 1
    Texture2D texture = LoadTexture("assets/tile1.png");
    textures.push_back(texture);

     // tile 2
    Texture2D texture2 = LoadTexture("assets/tile2.png");
    textures.push_back(texture2);
}
