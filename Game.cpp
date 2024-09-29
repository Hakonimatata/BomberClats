#include "Game.h"
#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include "GridMap.h"

using namespace std;

Game::Game(int winW, int winH, int numPlayers) : GridMap(), WinW(winW), WinH(winH), numPlayers(numPlayers)
{
    playerScores = vector<int>(numPlayers, 0);
}

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

    crownTexture = LoadTexture("assets/crown.png");

    // init weapons
    grenadeTexture = LoadTexture("assets/grenade.png");

    // Load background
    backgroundTexture = LoadTexture("assets/background.png");
   
    // Everything initialised correctly. Running.
    isRunning = true;
}



void Game::initPlayers()
{
    if (numPlayers >= 1 ) 
    {
        Player player = Player(100, 0, 1);
        player.init(PlayerControls{KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_RIGHT_CONTROL});
        players.push_back(player);
        playerTextures.push_back(LoadTexture("assets/spriteSheet1.png"));
    }
    
    if (numPlayers >= 2)
    {
        Player player2 = Player(200, 0, 2);
        player2.init(PlayerControls{KEY_A, KEY_D, KEY_W, KEY_LEFT_CONTROL});
        players.push_back(player2);
        playerTextures.push_back(LoadTexture("assets/spriteSheet2.png"));
    }

    if (numPlayers >= 3)
    {
        Player player3 = Player(300, 0, 3);
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
    int numPlayers = 0;
    int avgX = 0;
    int avgY = 0;
    for (Player player : players) 
    {
        if (player.GetHealth() <= 0 || player.getPosX() <= 0 || player.getPosY() <= 0 || player.getPosX() >= WinW || player.getPosY() >= WinH) continue;
        numPlayers++;
        avgX += player.getPosX();
        avgY += player.getPosY();
    }
    if (numPlayers == 0) return;
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

    if(IsKeyPressed(KeyboardKey::KEY_ESCAPE)){ isRunning = false; }

    if(IsKeyPressed(KeyboardKey::KEY_R)){ Reset(); }

    // Get player input from keyboard
    for (auto& player : players)
    {
        player.HandleInput(playerCommand);

        // Check players command
        if (playerCommand.useWeapon == UseWeapon::Grenade)
        {
            int throwerID = player.GetPlayerID();
            Grenade grenade = Grenade(playerCommand.posX, playerCommand.posY, playerCommand.velX, playerCommand.velY, throwerID);
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

    // Update grenades and handle logic
    UpdateGrenades(deltaTime);

    // Handle collisions
    HandleCollitions();

    // Update camera
    updateCamera();

    UpdateScore();
}

void Game::UpdateGrenades(float deltaTime)
{
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

    // Grenade logic
    for (auto& grenade : grenades) 
    {   
        // Check if player is within radius of explosion
        for (Player& player : players) 
        {
            // Auto explode on other players hitbox
            if (CheckCollisionRecs(grenade.getHitbox().GetRect(), player.getHitbox().GetRect())
                && player.GetPlayerID() != grenade.getThrowerID()  
                && !grenade.exploded &&  !player.IsDead())
            {
                grenade.explode();
            }
        }
        if(grenade.exploded && !grenade.inflictedDamage)
        {
            for (Player& player : players)
            {
                // Only do splash damage if no tiles are in between
                // if (!IsTileBetweenPoints({grenade.getPosX(), grenade.getPosY()}, {player.getPosX() + player.getHitbox().GetWidth() / 2, player.getPosY() + player.getHitbox().GetHeight() / 2}, map, tileSize, gridShiftX, gridShiftY)) 
                // {
                // }
                
                // get distance between player and grenade
                float dist = sqrt(pow(player.getPosX() - grenade.getPosX(), 2) + pow(player.getPosY() - grenade.getPosY(), 2));
                
                float splashDistance = 200.0f;
                if (dist < splashDistance) 
                {
                    float damage = grenade.getDamage() * (splashDistance - dist) / splashDistance;
                    player.InflictDamage(damage);
                }

            }
            grenade.inflictedDamage = true;
        }
    }
}

    

void Game::draw()
{
    // Draw background
    DrawTexturePro(backgroundTexture, { 0, 0, (float)backgroundTexture.width, (float)backgroundTexture.height }, { 0, 0, (float)WinW, (float)WinH }, { 0, 0 }, 0.0f, WHITE);

    // Draw playes
    for (int i = 0; i < players.size(); ++i) { players[i].Draw(playerTextures[i]); }

    // Draw health bars
    for (auto& player : players) { player.DrawHealthBar(player.getPosX(), player.getPosY() - 20, player.getWidth(), RED); }

    // Draw grenades
    for (auto& grenade : grenades) { grenade.Draw(grenadeTexture); }

    // Draw crown on leader
    if (currentLeaderIndex != -1) { players[currentLeaderIndex].DrawCrown(crownTexture); }

    // Draw player scores
    for (int i = 0; i < players.size(); ++i) { players[i].DrawScore(playerScores[i]); }

    DrawMap();
}

void Game::clean()
{
    // Unload textures
    for (auto& texture : playerTextures) { UnloadTexture(texture); }
    UnloadTexture(grenadeTexture);
    UnloadTexture(backgroundTexture);

}

void Game::Reset()
{
    for(auto& player : players) 
    {
        player.Reset();
    }
    gridShiftX = 0;
    gridShiftY = 0;
}

void Game::UpdateScore()
{
    int playersAlive = 0;
    int lastPlayerAliveIndex = 0; // For å holde styr på den siste gjenlevende spilleren

    for (int i = 0; i < players.size(); ++i) 
    {
        if (!players[i].IsDead()) 
        {
            ++playersAlive;
            lastPlayerAliveIndex = i; // Siste overlevende spiller
        }
    }

    // Increment only if only one player is alive
    if (playersAlive == 1 && !hasIncrementedScore) 
    {
        playerScores[lastPlayerAliveIndex]++;
        hasIncrementedScore = true; // Only increment once
    }

    // If players are alive, reset hasIncrementedScore
    if (playersAlive > 1) { hasIncrementedScore = false; }
    
    // Get player with highest score
    int highestScorePlayerIndex = distance(playerScores.begin(), max_element(playerScores.begin(), playerScores.end()));
    int highestScore = playerScores[highestScorePlayerIndex];

    // Check if unique
    int countHighestScore = count(playerScores.begin(), playerScores.end(), highestScore);

    if (countHighestScore > 1) // Not unique
    {
        // do not draw crown
        currentLeaderIndex = -1;
    }
    else // Unique, draw crown on this player
    {
        currentLeaderIndex = highestScorePlayerIndex;
    }
}