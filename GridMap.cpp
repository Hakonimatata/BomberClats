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

int GridMap::LoadLevel(const string& filename) {
    ifstream inFile(filename);
    if (!inFile) {
        cerr << "Failed to open file for loading: " << filename << endl;
        return -1;
    }

    inFile >> gridWidth >> gridHeight;

    // Resize map according to grid dimensions
    map.resize(gridHeight, vector<Tile>(gridWidth, Tile()));

    for (int y = 0; y < gridHeight; ++y) {
        for (int x = 0; x < gridWidth; ++x) {
            int tileId;
            inFile >> tileId;

            if (tileId != -1) {  // -1 represents empty tile
                Tile tile = Tile(tileId);
                map[y][x] = tile;

                if (tileId == 1) // 1 represents spawn point
                {
                    // Set spawn point
                    spawnPoints.push_back(FloatPoint({x * tileSize, y * tileSize}));
                }

            }
        }
    }

    inFile.close();

    return 0;
}

void GridMap::SaveLevel(const string& filename) const {
    ofstream ofs(filename);
    if (!ofs) {
        cerr << "Failed to open file for saving: " << filename << endl;
        return;
    }

    ofs << gridWidth << " " << gridHeight << endl;
    for (int y = 0; y < gridHeight; ++y) 
    {
        for (int x = 0; x < gridWidth; ++x) 
        {

            Tile tile = map.at(y).at(x);
            ofs << tile.id << " ";
        }
        
        ofs << endl;
    }

    ofs.close();
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

bool GridMap::isTileBetweenPoints(FloatPoint& point1, FloatPoint& point2)
{
    // distance between points
    float dx = point2.x - point1.x;
    float dy = point2.y - point1.y;

    // num steps is the max
    int steps = std::max(abs(dx), abs(dy)) / tileSize;

    // Get increment per step
    float xIncrement = dx / steps;
    float yIncrement = dy / steps;

    // start at point1 then iterate to point2
    float x = point1.x;
    float y = point1.y;

    // Check all steps on the line
    for (int i = 0; i <= steps; i++) 
    {
        // Get grid coordinates for current point
        int gridX = (x - gridShiftX) / tileSize;
        int gridY = (y - gridShiftY) / tileSize;

        // continue if invalid grid coordinates
        if (gridX < 0 || gridX >= gridWidth || gridY < 0 || gridY >= gridHeight) continue;

        // Return true if a tile is found at the point
        if (map[gridY][gridX].id != -1) 
        {
            return true;
        }

        // Next point
        x += xIncrement;
        y += yIncrement;
    }
    
    // No tiles found between point1 and point2, return false
    return false;
}
