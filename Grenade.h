#pragma once

#include <raylib.h>
#include "Hitbox.h"
#include "Utilities.h"

using namespace std;

class Grenade
{

    public:

        Grenade(float posX, float posY, float velX, float velY);
        ~Grenade();

        void Draw(Texture2D& texture);
        void Update(float deltaTime);
        void HandleCollisions(Hitbox& otherHitbox);

        Hitbox getHitbox() { return hitbox; }
        
        float getPosX() { return posX; }
        float getPosY() { return posY; }

        void setPosX(float posX) { this->posX = posX; }
        void setPosY(float posY) { this->posY = posY; }

        bool isDead = false;

    private:

        void explode();

        float size = 12.0f;
        float velX, velY, posX, posY;
        float g = 0.3f;
        Hitbox hitbox;

        float explodeTimer = 2.0f;
        float accumulatedTime = 0.0f;
        bool exploded = false;

};