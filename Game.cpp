#include "Game.h"
#include <iostream>
#include <string>
#include <iostream>
#include <fstream>
#include "GridMap.h"

using namespace std;

Game::Game(int winW, int winH, int numPlayers) : GridMap(), WinW(winW), WinH(winH), numPlayers(numPlayers)
{
    // Set up vector for scores
    playerScores = vector<int>(numPlayers, 0);

    // Initialize the map
    initTileSet();

    // Load level (Todo: error handling)
    LoadLevel("Levels/level.txt");
       
    initPlayers();

    // Load textures
    crownTexture = LoadTexture("assets/crown.png");
    grenadeTexture = LoadTexture("assets/grenade.png");
    backgroundTexture = LoadTexture("assets/background.png");
    groundTexture = LoadTexture("assets/ground.png");

    // Init 2D camera
    camera.target = (Vector2){WinW / 2.0f, WinH / 2.0f};
    camera.offset = (Vector2){WinW / 2.0f, WinH / 2.0f};
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
   
    // Everything initialised correctly. Running.
    isRunning = true;
}

Game::~Game()
{
    clean();
}

void Game::initPlayers()
{
    // Set generic spawnpoints for initializing players
    float spaceBetweenPlayers = gridWidth * tileSize / (numPlayers + 1);

    if (numPlayers >= 1 ) 
    {
        Player player = Player(spaceBetweenPlayers, 0, 1);
        player.init(PlayerControls{KEY_LEFT, KEY_RIGHT, KEY_UP, KEY_RIGHT_CONTROL});
        players.push_back(player);
        playerTextures.push_back(LoadTexture("assets/spriteSheet1.png"));
    }
    
    if (numPlayers >= 2)
    {
        Player player2 = Player(spaceBetweenPlayers * 2, 0, 2);
        player2.init(PlayerControls{KEY_A, KEY_D, KEY_W, KEY_LEFT_CONTROL});
        players.push_back(player2);
        playerTextures.push_back(LoadTexture("assets/spriteSheet2.png"));
    }

    if (numPlayers >= 3)
    {
        Player player3 = Player(spaceBetweenPlayers * 3, 0, 3);
        player3.init(PlayerControls{KEY_G, KEY_J, KEY_Y, KEY_SPACE});
        players.push_back(player3);
        playerTextures.push_back(LoadTexture("assets/spriteSheet3.png"));
    }

    // Set random spawn positions
    SetRandomSpawnPositions();
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

    // Kill out of bounds players
    for (int i = 0; i < players.size(); ++i) 
    {
        if (players[i].getPosY() > gridShiftY + gridHeight * tileSize)
        {
            players[i].SetHelth(0);
        }
    }
}

void Game::UpdateCamera()
{
    // Get target position for camera
    float avgX = 0;
    float avgY = 0;
    int numPlayersAlive = 0;
    for (Player& player : players) 
    {
        if (player.IsDead()) continue;
        numPlayersAlive++;
        avgX += player.getPosX();
        avgY += player.getPosY();
    }
    if (numPlayersAlive == 0) return;
    avgX /= (float)numPlayersAlive;
    avgY /= (float)numPlayersAlive;

    // Set target pos smoothly towards avg pos
    float lerpFactor = 0.05f;
    camera.target.x = camera.target.x + (avgX - camera.target.x) * lerpFactor;
    camera.target.y = camera.target.y + (avgY - camera.target.y) * lerpFactor;  

    // Camera zoom controls
    camera.zoom += ((float)GetMouseWheelMove()*0.05f);

    
    // Handle dynamic camera zoom
    float minZoom = 0.5f;
    float maxZoom = 3.0f;

    for (Player& player : players)
    {
        if (player.IsDead()) continue;
        // Transformer verdensposisjonen til skjermkoordinater
        Vector2 screenPos = GetWorldToScreen2D({player.getPosX(), player.getPosY()}, camera);

        // Sjekk om posisjonen er utenfor skjermens grenser
        bool outSideScreen = screenPos.x < 0 || screenPos.x + player.getWidth() > WinW || screenPos.y < 0 || screenPos.y + player.getHeight() > WinH;
        if (outSideScreen)
        {
            if(camera.zoom > minZoom) { camera.zoom -= 0.002f; }
        }
    }
    
}

void Game::HandleInput()
{
    if(IsKeyPressed(KeyboardKey::KEY_ESCAPE)){ isRunning = false; }

    if(IsKeyPressed(KeyboardKey::KEY_R)){ Reset(); }

    // Get player input from keyboard
    for (auto& player : players)
    {
        player.HandleInput(playerCommand);

        // Handle player command
        if (playerCommand.useWeapon == UseWeapon::Grenade)
        {
            int throwerID = player.GetPlayerID();
            Grenade grenade = Grenade(playerCommand.posX, playerCommand.posY, playerCommand.velX, playerCommand.velY, throwerID);
            grenades.push_back(grenade);
        }

        // Reset player commands
        playerCommand = PlayerCommand();
    }

    if(IsKeyPressed(KeyboardKey::KEY_Q)) { isRunning = false; }
}


void Game::Update(float deltaTime)
{
    // Update players
    for(auto& player : players) { player.Update(deltaTime); }

    // Update grenades and handle logic
    UpdateGrenades(deltaTime);

    // Handle collisions
    HandleCollitions();

    // Update camera
    UpdateCamera();

    // Update score
    UpdateScore();

    // Update reset timer
    UpdateResetTimer(deltaTime);
   
}

void Game::UpdateGrenades(float deltaTime)
{
    // Update grenades (position, timer etc.)
    for (int i = grenades.size() - 1; i >= 0; --i)
    {
        grenades.at(i).Update(deltaTime);
        // Remove if dead
        if (grenades.at(i).isDead)
        {
            grenades.erase(grenades.begin() + i);
        }
    }

    //--------------- Grenade logic ----------------

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
                FloatPoint playerMidPos = {player.getPosX() + player.getWidth() / 2, player.getPosY() + player.getHeight() / 2};
                FloatPoint grenadePos = {grenade.getPosX(), grenade.getPosY()};
                if (!isTileBetweenPoints(playerMidPos, grenadePos)) 
                {
                    // get distance between player and grenade
                    float dist = sqrt(pow(player.getPosX() + player.getWidth() / 2 - grenade.getPosX(), 2) + pow(player.getPosY() + player.getHeight() / 2 - grenade.getPosY(), 2));
                    
                    float splashDistance = 200.0f;
                    if (dist < splashDistance) 
                    {
                        float damage = grenade.getDamage() * (splashDistance - dist) / splashDistance;
                        player.InflictDamage(damage);
                    }
                }
            }
            // Update flag. Grenade can only do damage once
            grenade.inflictedDamage = true;
        }
    }
}

    

void Game::Draw()
{
    // Draw background
    DrawTexturePro(backgroundTexture, { 0, 0, (float)backgroundTexture.width, (float)backgroundTexture.height }, {0, 0, (float)WinW, (float)WinH }, { 0, 0 }, 0.0f, WHITE);

    // Draw on camera object
    BeginMode2D(camera);

        // Draw playes
        for (int i = 0; i < players.size(); ++i) { players[i].Draw(playerTextures[i]); }

        // Draw health bars
        for (auto& player : players) { player.DrawStatus(); }

        // Draw grenades
        for (auto& grenade : grenades) { grenade.Draw(grenadeTexture); }

        // Draw crown on leader
        if (currentLeaderIndex != -1) { players[currentLeaderIndex].DrawCrown(crownTexture); }

        // Draw player scores
        for (int i = 0; i < players.size(); ++i) { players[i].DrawScore(playerScores[i]); }

        DrawMap();

        // Draw ground
        DrawTexturePro(groundTexture, { 0, 0, (float)groundTexture.width, (float)groundTexture.height }, {-50, tileSize * gridHeight - 70, tileSize * gridWidth + 100, (float) WinH / 2.0f }, { 0, 0 }, 0.0f, WHITE);

    EndMode2D();
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

    camera.zoom = 1.0f;

    SetRandomSpawnPositions();
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

void Game::UpdateResetTimer(float deltaTime)
{
    if (numPlayers == 1) { return; } // No need for reset timer if only one player
    int playersAlive = 0;
    for (Player& player : players) 
    { 
        if (!player.IsDead()) 
        { 
            ++playersAlive; 
        } 
    }

    if (resetTimer > 0.0f && playersAlive <= 1) { resetTimer -= deltaTime; }
    if (resetTimer <= 0.0f) 
    { 
        Reset();
        resetTimer = resetDelay; 
    }
}

void Game::SetRandomSpawnPositions()
{
    if (numPlayers > spawnPoints.size()) return; // Too many players for spawnpoints
    // Seed random generator
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(spawnPoints.begin(), spawnPoints.end(), g);

    float heightOffset = -100; // distance above spawn points
    for (int i = 0; i < players.size(); ++i) 
    {
        players[i].SetSpawnPos(spawnPoints[i].x, spawnPoints[i].y + heightOffset);
    }
}
