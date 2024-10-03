#pragma once

#include <raylib.h>
#include "Hitbox.h"
#include "Utilities.h"
#include <iostream>
#include <vector>
#include "AnimationHandler.h"

using namespace std;

// Helping enums
enum class PlayerFacing 
{
    left,
    right
};
enum class SelectedWeapon
{
    Grenade,
};

class Player : public AnimationHandler
{
    public:

        Player(float posX, float posY, int playerID);
        ~Player();
        
        void init(PlayerControls controls);

        void HandleInput(PlayerCommand& playerCommand);
        void Update(float deltaTime);
        void Draw(Texture2D& texture);
        void DrawCrown(Texture2D& texture);
        void DrawScore(int score);
        void DrawStatus();

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
        void SetSpawnPos(float spawnPosX, float spawnPosY) { this->spawnPosX = spawnPosX; this->spawnPosY = spawnPosY; }

        void HandleCollisions(Hitbox& otherHitbox);

        void InflictDamage(float damage) { health -= damage; }
        float GetHealth() { return health; }
        void SetHelth(float health) { this->health = health; }
        bool IsDead() { return isDead; }
        int GetPlayerID() { return playerID; }

        void Reset();

    private:
        void jump();
        void fall();
        void throwGrenade(PlayerCommand& playerCommand);
        void die();

        float width = 40;
        float height = 40;
        float spawnPosX, spawnPosY;
        float posX, posY;
        float velX, velY;
        float maxFallVel = 10.0f;
        int playerID;

        // Player state
        const int numJumps = 2;
        int jumpsLeft;
        PlayerFacing playerFacing = PlayerFacing::right;
        SelectedWeapon selectedWeapon = SelectedWeapon::Grenade;
        bool onGround = false;
        float health = 100.0f;
        bool isDead = false;

        // Grenade capacity and cooldown for player
        const float grenadeThrowDelay = 1.0f; // 1 second
        float grenadeThrowTimer = 0.0f; // 0 is ready to throw
        
        float grenadeCharge = 0.0f;
        float maxGrenadeCharge = 6.0f;
        
        // Animation
        Rectangle getSourceRect(Texture2D& texture);

        

        Hitbox hitbox;
        PlayerControls controls;
};