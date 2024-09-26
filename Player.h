#pragma once

#include <raylib.h>
#include "Hitbox.h"
#include "Utilities.h"
#include <iostream>
#include <vector>

using namespace std;

// Helping enum for player facing
enum class PlayerFacing 
{
    left,
    right
};

class Player 
{
    public:

        Player(float posX, float posY);
        ~Player();
        
        void init(PlayerControls controls);

        void HandleInput(PlayerCommand& playerCommand);
        void Update();
        void Draw(Texture2D& texture);

        Hitbox getHitbox() { return hitbox; }
        float getPosX() { return posX; }
        float getPosY() { return posY; }
        float getVelX() { return velX; }
        float getVelY() { return velY; }
        float getHeight() { return height; }
        float getWidth() { return width; }
        PlayerFacing getFacing() { return playerFacing; }

        void setPosX(float posX) { this->posX = posX; }
        void setPosY(float posY) { this->posY = posY; }
        void setVelX(float velX) { this->velX = velX; }
        void setVelY(float velY) { this->velY = velY; }

        void HandleCollisions(Hitbox& otherHitbox);

    private:
        void jump();
        void fall();
        void throwGrenade(PlayerCommand& playerCommand);

        float width = 40;
        float height = 40;
        float posX, posY;
        float velX, velY;
        float maxFallVel = 100.0f;

        // Player state
        const int numJumps = 2;
        int jumpsLeft;
        PlayerFacing playerFacing = PlayerFacing::right;
        bool onGround = false;
        
        // Animation
        int currentFrame = 0;
        int framesCounter = 0;
        const int frameUpdateRate = 5;
        Rectangle getSourceRect(Texture2D& texture);

        Hitbox hitbox;
        PlayerControls controls;
};