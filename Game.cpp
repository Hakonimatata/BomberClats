#include "Game.h"
#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include "GridMap.h"

using namespace std;

Game::Game(int winW, int winH, int numPlayers) : GridMap(), WinW(winW), WinH(winH), numPlayers(numPlayers)
{}

Game::~Game()
{
    clean();
}

void Game::init()
{
    // Initialize the map
    initTileSet();

    initPlayers();

    LoadLevel("Levels/level.txt");

    // init weapons
    grenadeTexture = LoadTexture("assets/grenade.png");

    // Everything initialised correctly. Running.
    isRunning = true;
}



void Game::initPlayers()
{
    if (numPlayers >= 1 ) 
    {
        Player player = Player(100, 0);
        player.init(PlayerControls{KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_RIGHT_CONTROL});
        players.push_back(player);
        playerTextures.push_back(LoadTexture("assets/spriteSheet1.png"));
    }
    
    if (numPlayers >= 2)
    {
        Player player2 = Player(200, 0);
        player2.init(PlayerControls{KEY_A, KEY_D, KEY_W, KEY_Q});
        players.push_back(player2);
        playerTextures.push_back(LoadTexture("assets/spriteSheet2.png"));
    }

    if (numPlayers >= 3)
    {
        Player player3 = Player(300, 0);
        player3.init(PlayerControls{KEY_J, KEY_L, KEY_I, KEY_SPACE});
        players.push_back(player3);
        playerTextures.push_back(LoadTexture("assets/spriteSheet3.png"));
    }
}

void Game::resize(int newWidth, int newHeight)
{
    // Set new window size 
    WinW = newWidth;
    WinH = newHeight;
}

void Game::HandleCollitions()
{   
    // Handle collitions with tiles
    for (int tileY = 0; tileY < gridHeight; ++tileY) 
    {
        for (int tileX = 0; tileX < gridWidth; ++tileX) 
        {
            Tile tile = map.at(tileY).at(tileX);
            if (tile.id == -1) continue;

            FloatPoint tilePos = GetPosFromGridCoords(gridShiftX, gridShiftY, tileX, tileY, tileSize);
            Hitbox tileHitbox = Hitbox{tilePos.x, tilePos.y, tileSize, tileSize};

            // Check if grenades and tile collided
            for (Grenade& grenade : grenades)
            {
                if(CheckCollisionRecs(tileHitbox.GetRect(), grenade.getHitbox().GetRect()))
                {
                    grenade.HandleCollisions(tileHitbox);
                }
            }

            // Check if player and tile collided
            for (Player& player : players)
            {
                if(CheckCollisionRecs(tileHitbox.GetRect(), player.getHitbox().GetRect()))
                {
                    player.HandleCollisions(tileHitbox);
                }
            }
        }
    }
}

void Game::updateCamera()
{
    // Desired mid point
    int midX = WinW / 2;
    int midY = WinH / 2;

    // get center point of all players
    int numPlayers = players.size();
    int avgX = 0;
    int avgY = 0;
    for (Player player : players) 
    {
        avgX += player.getPosX();
        avgY += player.getPosY();
    }
    avgX /= numPlayers;
    avgY /= numPlayers;

    // move everything smoothly towards midPoint
    float lerpFactor = 0.1f;
    gridShiftX += (midX - avgX) * lerpFactor;
    gridShiftY += (midY - avgY) * lerpFactor;

    // move players as well
    for (Player& player : players) 
    {
        float newPosX = player.getPosX() + (midX - avgX) * lerpFactor;
        float newPosY = player.getPosY() + (midY - avgY) * lerpFactor;

        player.setPosX(newPosX);
        player.setPosY(newPosY);
    }

    // Move grenades
    for (Grenade& grenade : grenades) 
    {
        float newPosX = grenade.getPosX() + (midX - avgX) * lerpFactor;
        float newPosY = grenade.getPosY() + (midY - avgY) * lerpFactor;

        grenade.setPosX(newPosX);
        grenade.setPosY(newPosY);
    }
}

void Game::handleEvents()
{
    // Handle window resizing
    if (IsWindowResized()) 
    {
        // Get new window size
        int newWidth = GetScreenWidth();
        int newHeight = GetScreenHeight();

        // Resize elements before setting new window size.
        resize(newWidth, newHeight);
    }

    if(IsKeyPressed(KeyboardKey::KEY_ESCAPE))
    {
        isRunning = false;
    }

    // Get player input from keyboard
    for (auto& player : players)
    {
        player.HandleInput(playerCommand);

        // Check players command
        if (playerCommand.useWeapon == UseWeapon::Grenade)
        {
            Grenade grenade = Grenade(playerCommand.posX, playerCommand.posY, playerCommand.velX, playerCommand.velY);
            grenades.push_back(grenade);
        }

        // Reset player commands
        playerCommand = PlayerCommand();
    }
}


void Game::update(float deltaTime)
{
    // Update players
    for(auto& player : players) { player.Update(); }

    // Update grenades
    for (int i = grenades.size() - 1; i >= 0; --i)
    {
        grenades.at(i).Update(deltaTime);
        // Temp: Remove if out of bounds
        if (grenades.at(i).getPosX() > WinW || grenades.at(i).getPosX() < 0 || grenades.at(i).getPosY() > WinH || grenades.at(i).getPosY() < 0)
        {   
            grenades.erase(grenades.begin() + i);
        }
        // Remove if dead
        else if (grenades.at(i).isDead)
        {
            grenades.erase(grenades.begin() + i);
        }
    }

    // Handle collisions
    HandleCollitions();

    // Update camera
    updateCamera();
}

    

void Game::draw()
{
    // Draw playes
    for (int i = 0; i < players.size(); ++i) { players[i].Draw(playerTextures[i]); }

    // Draw grenades
    for (auto& grenade : grenades) { grenade.Draw(grenadeTexture); }

    DrawMap();
}

void Game::clean()
{
    // Unload textures
    for (auto& texture : playerTextures) { UnloadTexture(texture); }
    
}