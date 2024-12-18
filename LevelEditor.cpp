#include "LevelEditor.h"

LevelEditor::LevelEditor() : GridMap()
{
}

LevelEditor::~LevelEditor()
{
    map.clear();
}

void LevelEditor::init(int width, int height, Context context)
{
    // Set window size
    WinW = width;
    WinH = height;

    // Set  grid shift based on window size
    gridShiftX = WinW * 0.1;
    gridShiftY = WinH * 0.1;

    gridEndX = WinW * 0.8;
    
    tileSize = 32; // Sets the size of the tiles in the editor

    // Initialize the set of tiles
    initTileSet();

    // create empty map
    // CreateEmptyMap(context.gridWidth, context.gridHeight);
    
    // Load level
    if (LoadLevel("Levels/level.txt") == -1)  
    {
        cout << "Failed to load level" << endl;
    }

    // CreateEmptyMap(context.gridWidth, context.gridHeight);


    // Init buttons
    float buttonWidth = 50;
    float buttonHeight = buttonWidth * 0.7;
    exitButton = Button("assets/Buttons/exitButton.png", gridEndX + 10, 10, buttonWidth, buttonHeight);
    saveButton = Button("assets/Buttons/saveButton.png", gridEndX + buttonWidth + 20, 10, buttonWidth, buttonHeight);

    // ----------Level editor running---------------
    isRunning = true;
}



void LevelEditor::HandleInput()
{

    if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
    {
        RemoveTile();
    }
    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) 
    {
        // Handle left click
        SelectTile();
        
        PlaceTile();

        if (exitButton.isClicked(mouseX, mouseY))
        {
            isRunning = false;
        }
        else if (saveButton.isClicked(mouseX, mouseY))
        {
            SaveLevel("Levels/level.txt");
        }
    }

    MooveCamera();
}

void LevelEditor::MooveCamera()
{
    // Move camera
    if(IsKeyDown(KEY_LEFT)) 
    {
        gridShiftX += 10;
    }
    else if(IsKeyDown(KEY_RIGHT))
    {
        gridShiftX -= 10;
    }
    if(IsKeyDown(KEY_UP))
    {
        gridShiftY += 10;
    }
    else if(IsKeyDown(KEY_DOWN))
    {
        gridShiftY -= 10;
    }
}

void LevelEditor::Update()
{
    // update mouse position
    mouseX = GetMouseX();
    mouseY = GetMouseY();
}


void LevelEditor::Draw()
{
    // Add to renderer here
    DrawGrid();

    // Draw map and tiles
    DrawMap();

    // Draw menu background
    DrawRectangle(gridEndX, 0, WinW - gridEndX, WinH, { 0, 0, 0, 150 });

    // Draw available tiles
    DrawAvailableTiles();

    DrawSelectedTile();

    //draw buttons
    exitButton.Draw();
    saveButton.Draw();
}


void LevelEditor::PlaceTile() {

    if(mouseX > gridEndX) return;

    int gridX = (mouseX - gridShiftX) / tileSize;
    int gridY = (mouseY - gridShiftY) / tileSize;

    // Check if in bounds (prevent tile to render outside)
    if (gridX >= 0 && gridX < gridWidth && gridY >= 0 && gridY < gridHeight) 
    {
        // Place tile on map
        map.at(gridY).at(gridX) = selectedTile;
    }   
}

void LevelEditor::RemoveTile() {

    int gridX = (mouseX - gridShiftX) / tileSize;
    int gridY = (mouseY - gridShiftY) / tileSize;

    // Check if in bounds (prevent tile to render outside)
    if (gridX >= 0 && gridX < gridWidth && gridY >= 0 && gridY < gridHeight) 
    {
        // Remove tile on map
        map.at(gridY).at(gridX) = Tile(-1);
    }   
}

void LevelEditor::SelectTile()
{
    // loop through avalable tiles and check if mouse is inside
    for (int id = 0; id < tileSet->textures.size(); ++id) 
    {
        Rectangle rect = GetAvailableTileRect(id);

        if (mouseX >= rect.x && mouseX <= rect.x + rect.width && mouseY >= rect.y && mouseY <= rect.y + rect.height) {
            selectedTile = Tile(id);
        }
    }
}

void LevelEditor::DrawSelectedTile()
{
    if (mouseX > gridEndX) return; // only draw if in editor grid
    // Get selected tile texture 
    int selectedTileId = selectedTile.id;
    if (selectedTileId == -1) return;

    Texture2D texture = tileSet->textures.at(selectedTileId);

    // return if outside grid
    if (mouseX < gridShiftX || mouseX > gridShiftX + gridWidth * tileSize || mouseY < gridShiftY || mouseY > gridShiftY + gridHeight * tileSize) {
        return;
    }

    // get grid position
    int gridX = (mouseX - gridShiftX) / tileSize;
    int gridY = (mouseY - gridShiftY) / tileSize;

    // Get selected tile position
    float posX = (gridX * tileSize) + gridShiftX;
    float posY = (gridY * tileSize) + gridShiftY;

    // Draw selected tile
    DrawTexturePro(texture, { 0, 0, (float)texture.width, (float)texture.height }, { posX, posY, tileSize, tileSize }, { 0, 0 }, 0.0f, WHITE);
}


void LevelEditor::DrawAvailableTiles() const
{
    for (int id = 0; id < tileSet->textures.size(); ++id) 
    {

        Rectangle rect = GetAvailableTileRect(id);

        Texture2D texture = tileSet->textures.at(id);

        DrawTexturePro(texture, { 0, 0, (float)texture.width, (float)texture.height }, rect, { 0, 0 }, 0.0f, WHITE);
    }
}

Rectangle LevelEditor::GetAvailableTileRect(int tileIndex) const
{
    int xShift = 40;
    int yShift = 100;
    int spaceBetweenTiles = 5;

    int maxRows = (WinH - 2 * yShift) / (tileSize  + spaceBetweenTiles);
    if (maxRows <= 0) { maxRows = 1; } // prevents crash

    int xPosition = WinW - xShift - (tileSize  + spaceBetweenTiles) - (tileSize + spaceBetweenTiles) * (tileIndex / maxRows);
    int yPosition = yShift + (tileIndex % maxRows) * (tileSize + spaceBetweenTiles);

    Rectangle rect;
    rect.x = xPosition;
    rect.y = yPosition;
    rect.width = tileSize;
    rect.height = tileSize;

    return rect;
}