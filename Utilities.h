#pragma once

struct FloatPoint{
    float x;
    float y;
    void normalize();
    float getSize();
};

float getDistanceBetween(FloatPoint a, FloatPoint b);

FloatPoint getMidPointBetween(FloatPoint a, FloatPoint b);

struct Point{
    int x;
    int y;
};

struct PlayerControls {
    int moveLeft;   // Tastkode for å bevege til venstre
    int moveRight;  // Tastkode for å bevege til høyre
    int jump;       // Tastkode for å hoppe
    int useWeapon;
};

enum class MenuSelection {
    None, 
    StartGame,
    StartLevelEditor,
};

struct Context 
{
    int numPlayers = 1;
    MenuSelection menuSelection = MenuSelection::None;
    
    // For level editor
    int gridWidth = 40;
    int gridHeight = 20;

    bool editExistingMap = false;
};


enum class UseWeapon
{
    None,
    Grenade
};

struct PlayerCommand
{
    UseWeapon useWeapon = UseWeapon::None;   
    float posX;
    float posY;
    float velX;
    float velY;
};