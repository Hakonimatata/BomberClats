#include <iostream>
#include <raylib.h>
#include "Game.h"
#include "Menu.h"
#include "LevelEditor.h"

using namespace std;



int main () {

    const int screenWidth = 1400;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "2D Shooter");
    SetTargetFPS(60);

    // global context (communication between menu and game)
    Context context = Context();

    while (WindowShouldClose() == false)
    {
        // Menu loop
        Menu menu = Menu(screenWidth, screenHeight, context);
        while(menu.Running() && !WindowShouldClose())
        {
            BeginDrawing();
            ClearBackground(BLACK);

                menu.HandleButtonClicks(context);
                menu.Update();
                menu.Draw();
            
            EndDrawing();
        }

        switch (context.menuSelection)
        {
            case MenuSelection::StartLevelEditor:
            {
                LevelEditor levelEditor = LevelEditor();
                levelEditor.init(screenWidth, screenHeight, context);
                while (levelEditor.running() && !WindowShouldClose()) 
                {
                    BeginDrawing();
                    ClearBackground(BLACK);
                    
                        levelEditor.HandleInput();
                        levelEditor.Update();
                        levelEditor.Draw();
                    
                    EndDrawing();
                }
            }   
            break;

            case MenuSelection::StartGame:
            {
                // ToggleBorderlessWindowed();
                
                // int screenWidth = GetMonitorWidth(0);  
                // int screenHeight = GetMonitorHeight(0);
                // SetWindowSize(screenWidth, screenHeight);
                // ToggleFullscreen();
                
                // SetWindowSize(screenWidth * 2, screenHeight);
                Game game = Game(screenWidth, screenHeight, context.numPlayers);

                // Game loop
                while (game.Running() && !WindowShouldClose())
                {
                    BeginDrawing();
                    ClearBackground(WHITE);
                    
                        game.HandleInput();
                        game.Update(GetFrameTime());
                        game.Draw();

                    EndDrawing();
                }
            }   
            break;

            default:
                break;
        }
    }

    CloseWindow();
    return 0;
}