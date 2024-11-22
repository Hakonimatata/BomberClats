#pragma once

#include <stdio.h>
#include <string>
#include <vector>
#include "Player.h"
#include "TileSet.h"
#include "Utilities.h"
#include "GridMap.h"
#include "Grenade.h"
#include <math.h>
#include <algorithm>
#include <ctime> 
#include <random>
#include "Sounds.h"

using namespace std;

class Game : public GridMap
{
    public:
        Game(int winW, int winH, int numPlayers);
        ~Game();

        void HandleInput();
        void Update(float deltaTime);
        void Draw();
        void clean();

        bool Running(){ return isRunning; }

    private:
        int WinW, WinH;                   // Original width and height
        bool isRunning;                   // Is game running
        int currentLeaderIndex = -1;      // -1 if no unique leader
        bool hasIncrementedScore = false;
        
        // Functions
        void initPlayers();
        void HandleCollitions();
        void UpdateCamera();
        void Reset();
        void UpdateGrenades(float deltaTime);
        void UpdateScore();
        void UpdateResetTimer(float deltaTime);
        void SetRandomSpawnPositions();

        // Players
        int numPlayers;
        vector<Player> players;
        PlayerCommand playerCommand;
        vector<Texture2D> playerTextures;
        vector<int> playerScores;

        // Background
        Texture2D backgroundTexture;      // Background texture
        Texture2D groundTexture;          // Ground texture

        // Grenades
        vector<Grenade> grenades;
        Texture2D grenadeTexture;         // grenadeTexture

        // Reset timer
        const float resetDelay = 5.0f;    // Wait 5 seconds before reset
        float resetTimer = 0.0f;

        Texture2D crownTexture;
        
        Camera2D camera;
};