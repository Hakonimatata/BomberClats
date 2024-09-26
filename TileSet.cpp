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
    Texture2D texture = LoadTexture("assets/tile1.png");
    // load tiles into textures map
    textures.push_back(texture);
}
