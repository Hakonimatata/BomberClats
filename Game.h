#pragma once

#include <stdio.h>
#include <string>
#include <vector>
#include "Player.h"
#include "TileSet.h"
#include "Utilities.h"
#include "GridMap.h"
#include "Grenade.h"

using namespace std;

class Game : public GridMap
{
    public:
        Game(int winW, int winH, int numPlayers);
        ~Game();

        void init();
        
        void handleEvents();
        void update(float deltaTime);
        void draw();
        void clean();

        bool Running(){ return isRunning; }

    private:
        int WinW, WinH;                   // Original width and height
        bool isRunning;                   // Is game running
        
        // Functions
        void initPlayers();
        void resize(int newWidth, int newHeight);
        void HandleCollitions();
        void updateCamera();

        // Players
        int numPlayers;
        vector<Player> players;
        PlayerCommand playerCommand;
        vector<Texture2D> playerTextures;

        // Background
        Rectangle backgroundSpriteRect;   // For background if needed
        Texture2D backgroundTexture;      // Background texture

        // Grenades
        vector<Grenade> grenades;
        Texture2D grenadeTexture;         // grenadeTexture
};