#pragma once

#include <vector>
#include <string>
#include "TileSet.h"
#include "Utilities.h"
#include <iostream>
#include <fstream>
#include "GridMap.h"
#include "Button.h"

using namespace std;

class LevelEditor : public GridMap
{
public:
    LevelEditor();
    ~LevelEditor();

    void HandleInput();
    void init(int width, int height, Context context);
    void Update();
    void Draw();

    bool running() { return isRunning; }


private:
    void PlaceTile();
    void RemoveTile();

    void MooveCamera();
    void DrawAvailableTiles() const;
    void SelectTile();
    void DrawSelectedTile();
    Rectangle GetAvailableTileRect(int id) const;

    Tile selectedTile;
    bool isRunning;
    int WinW, WinH;
    int mouseX, mouseY;

    float gridEndX;


    Button exitButton;
    Button saveButton;
};