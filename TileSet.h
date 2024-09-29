#pragma once

#include <vector>
#include <raylib.h> 

using namespace std;


class TileSet {
    public:
        TileSet();
        ~TileSet();
        void init();
        vector<Texture2D> textures;
};

class Tile
{
    public: 
        Tile(int id) : id(id) {};
        Tile() = default;
        int id = -1;
};