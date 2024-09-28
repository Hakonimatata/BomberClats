#include "GridMap.h"
#include <iostream>

GridMap::GridMap()
{
    
}

GridMap::~GridMap()
{   
    delete tileSet;
}



void GridMap::initTileSet()
{
    tileSet = new TileSet();
    tileSet->init();
    gridShiftX = 0;
    gridShiftY = 0;
}

void GridMap::DrawMap() const
{
    for (int y = 0; y < gridHeight; ++y) {
        for (int x = 0; x < gridWidth; ++x) {

            Tile tile = map.at(y).at(x);

            if (tile.id == -1) continue;

            // draw tile
            Texture2D texture = tileSet->textures.at(tile.id);

            float xPos = gridShiftX + x * tileSize;
            float yPos = gridShiftY + y * tileSize;

            // Draw tile
            Rectangle sourceRec = { 0.0f, 0.0f, (float)texture.width, (float)texture.height };
            Rectangle destRec = { xPos, yPos, tileSize, tileSize };
            DrawTexturePro(texture, sourceRec, destRec, { 0, 0 }, 0.0f, WHITE);

        }
    }
}

void GridMap::CreateEmptyMap(int gridWidth, int gridHeight)
{
    this->gridWidth = gridWidth;
    this->gridHeight = gridHeight;

    // Resize map according to grid dimensions
    map.resize(gridHeight, std::vector<Tile>(gridWidth, Tile()));

    for (int y = 0; y < gridHeight; ++y) {
        for (int x = 0; x < gridWidth; ++x) {
            Tile tile = Tile(-1);
            map[y][x] = tile;
        }
    }
}

int GridMap::LoadLevel(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile) {
        std::cerr << "Failed to open file for loading: " << filename << std::endl;
        return -1;
    }

    inFile >> gridWidth >> gridHeight;

    // Resize map according to grid dimensions
    map.resize(gridHeight, std::vector<Tile>(gridWidth, Tile()));

    for (int y = 0; y < gridHeight; ++y) {
        for (int x = 0; x < gridWidth; ++x) {
            int tileId;
            inFile >> tileId;

            if (tileId != -1) {  // -1 representerer ingen tile
                Tile tile = Tile(tileId);
                map[y][x] = tile;
            }
        }
    }

    inFile.close();

    return 0;
}

void GridMap::SaveLevel(const std::string& filename) const {
    std::ofstream outFile(filename);
    if (!outFile) {
        std::cerr << "Failed to open file for saving: " << filename << std::endl;
        return;
    }

    outFile << gridWidth << " " << gridHeight << std::endl;
    for (int y = 0; y < gridHeight; ++y) {
        for (int x = 0; x < gridWidth; ++x) {
            
            int tileId;

            Tile tile = map.at(y).at(x);

           
            tileId = tile.id;
            
            outFile << tileId << " ";
        }
        outFile << std::endl;
    }

    outFile.close();
}

void GridMap::DrawGrid() const
{
    ClearBackground(RAYWHITE);  // Background color

    Color gridColor = {20, 20, 20, 255};  // Grid line color

    // Draw vertical lines
    for (int i = 0; i <= gridWidth; ++i) {
        DrawLine(gridShiftX + i * tileSize, gridShiftY, gridShiftX + i * tileSize, gridShiftY + gridHeight * tileSize, gridColor);
    }

    // Draw horizontal lines
    for (int i = 0; i <= gridHeight; ++i) {
        DrawLine(gridShiftX, gridShiftY + i * tileSize, gridShiftX + gridWidth * tileSize, gridShiftY + i * tileSize, gridColor);
    }
}

FloatPoint GridMap::GetPosFromGridCoords(float gridshiftX, float gridshiftY, int gridX, int gridY, int tileSize)
{
    float posX = (gridX * tileSize) + gridshiftX;
    float posY = (gridY * tileSize) + gridshiftY;

    return {posX, posY};
}
