#include <iostream>
#include <raylib.h>
#include "Game.h"
#include "Menu.h"

using namespace std;



int main () {

    const int screenWidth = 800;
    const int screenHeight = 600;

    InitWindow(screenWidth, screenHeight, "2D Shooter");
    SetTargetFPS(60);

    // global context (communication between menu and game)
    Context context = Context();

    while (WindowShouldClose() == false)
    {
        // Menu loop
        Menu menu = Menu(screenWidth, screenHeight);
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
            case MenuSelection::StartGame:
            {
                // ToggleBorderlessWindowed();
                
                Game game = Game(screenWidth, screenHeight, context.numPlayers);
                game.init();

                while (game.Running() && !WindowShouldClose())
                {
                    BeginDrawing();
                    ClearBackground(WHITE);
                    
                        game.handleEvents();
                        game.update(GetFrameTime());
                        game.draw();

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