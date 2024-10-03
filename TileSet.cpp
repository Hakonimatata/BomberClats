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
    Texture2D texture = LoadTexture("assets/Tiles/tile1.png");
    textures.push_back(texture);

    // tile 2
    Texture2D texture2 = LoadTexture("assets/Tiles/tile2.png");
    textures.push_back(texture2);

    // tile 3
    Texture2D texture3 = LoadTexture("assets/Tiles/tile3.png");
    textures.push_back(texture3);

    // tile 4
    Texture2D texture4 = LoadTexture("assets/Tiles/tile4.png");
    textures.push_back(texture4);

    // tile 5
    Texture2D texture5 = LoadTexture("assets/Tiles/tile5.png");
    textures.push_back(texture5);

    // tile 6
    Texture2D texture6 = LoadTexture("assets/Tiles/tile6.png");
    textures.push_back(texture6);

    // tile 7
    Texture2D texture7 = LoadTexture("assets/Tiles/tile7.png");
    textures.push_back(texture7);

    // tile 8
    Texture2D texture8 = LoadTexture("assets/Tiles/tile8.png");
    textures.push_back(texture8);

    // tile 9
    Texture2D texture9 = LoadTexture("assets/Tiles/tile9.png");
    textures.push_back(texture9);

    // tile 10
    Texture2D texture10 = LoadTexture("assets/Tiles/tile10.png");
    textures.push_back(texture10);

    // tile 11
    Texture2D texture11 = LoadTexture("assets/Tiles/tile11.png");
    textures.push_back(texture11);
}
