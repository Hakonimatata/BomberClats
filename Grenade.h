#pragma once

#include <raylib.h>
#include "Hitbox.h"
#include "Utilities.h"
#include <iostream>
#include "AnimationHandler.h"

using namespace std;

class Grenade : public AnimationHandler
{

    public:

        Grenade(float posX, float posY, float velX, float velY, int throwerID);
        ~Grenade();

        void Draw(Texture2D& texture);
        void Update(float deltaTime);
        void HandleCollisions(Hitbox& otherHitbox);

        Hitbox getHitbox() { return hitbox; }
        
        float getPosX() { return posX; }
        float getPosY() { return posY; }
        float getDamage() { return damage; }
        int getThrowerID() { return throwerID; }

        void setPosX(float posX) { this->posX = posX; }
        void setPosY(float posY) { this->posY = posY; }

        void explode();
        bool isDead = false;
        bool exploded = false;
        bool inflictedDamage = false;

    private:

        float damage = 35.0f;
        float size = 12.0f;
        float g = 0.3f;
        int throwerID; 
        float velX, velY, posX, posY;
        Hitbox hitbox;

        // Timers etc.
        float explodeTimer = 2.0f;
        float accumulatedTime = 0.0f;
        int numFramesForExplotion = 12;

        // Animation
        Rectangle getSourceRect(Texture2D& texture);

};