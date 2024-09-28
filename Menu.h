#pragma once

#include <raylib.h>
#include "Button.h"
#include "Utilities.h"
#include <vector>
#include <iostream>

using namespace std;


class Menu 
{
    public:
        Menu(int winW, int winH);
        ~Menu() = default;

        void Draw();
        void Update();
        void HandleButtonClicks(Context& context);
        bool Running() { return isRunning; }

        void AddPlayer(Context& context);
        void RemovePlayer(Context& context);

    private:

        int winW, winH;
        bool isRunning;

        // Buttons:
        Button startButton;
        Button addPlayerButton;
        Button removePlayerButton;
        Button startLevelEditorButton;


        // Display Players:
        vector<Texture2D> playerTextures;
        vector<FloatPoint> playerPositions;
        float playerSize = 100.0f;
        void DrawPlayers();
        void UpdateCamera();
        

};