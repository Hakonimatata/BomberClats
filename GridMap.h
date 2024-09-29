#pragma once

#include "TileSet.h"
#include <vector>
#include "TileSet.h"
#include <fstream>
#include "Utilities.h"

class GridMap
{
    public:
        GridMap();
        ~GridMap();


        void initTileSet();
        void DrawMap() const;  

        int LoadLevel(const string& filename);
        void SaveLevel(const string& filename) const;

        void DrawGrid() const;

        void CreateEmptyMap(int gridWidth, int gridHeight);

        static FloatPoint GetPosFromGridCoords(float gridshiftX, float gridshiftY, int gridX, int gridY, int tileSize);


        int gridWidth;
        int gridHeight;
        float gridShiftX;
        float gridShiftY;
        float tileSize = 32;
        vector<vector<Tile>> map;
        TileSet* tileSet;
    private:
};
