#include "Grenade.h"


Grenade::Grenade(float posX, float posY, float velX, float velY) : 
    posX(posX), posY(posY), velX(velX), velY(velY), hitbox(posX, posY, size, size)
{
    
}

Grenade::~Grenade()
{
    // Todo: find out how to unload textures for multiple instances of
    // the same texture. Texture manager class?
}



void Grenade::Draw(Texture2D& texture)    
{
    DrawTexturePro(texture, { 0.0f, 0.0f, (float)texture.width, (float)texture.height }, 
        { posX, posY, size, size }, { 0, 0 }, 0.0f, WHITE);
}


void Grenade::Update(float deltaTime)
{
    accumulatedTime += deltaTime;

    velY += g;
    posX += velX;
    posY += velY;

    // Update hitbox position
    hitbox = Hitbox(posX, posY, size, size);

    // Update explode
    if(accumulatedTime > explodeTimer)
    {
        explode();
    }
}

void Grenade::explode()
{
    // Stop moving
    velX = 0;
    velY = 0;
    exploded = true;

    // change to explode frame for split secound and then set as dead

    isDead = true;
}

void Grenade::HandleCollisions(Hitbox& otherHitbox)
{
    // Four points to check for collition
    FloatPoint topPoint = FloatPoint{posX + size / 2.0f, posY};
    FloatPoint bottomPoint = FloatPoint{posX + size / 2.0f, posY + size};
    FloatPoint leftPoint = FloatPoint{posX, posY + size / 2.0f};
    FloatPoint rightPoint = FloatPoint{posX + size, posY + size / 2.0f};

    // Check if point is within tile, and act accordingly

    float friction = 0.7f;

    // Floor collition
    if(Hitbox::isPointWithin(bottomPoint, otherHitbox))
    {
        // Handle floor collision
        velY *= -1 * friction;
        velX *= friction;
        posY--;
    }

    else if (Hitbox::isPointWithin(topPoint, otherHitbox)) 
    {
        // Handle ceiling collision
        velY *= -1 * friction;
        velX *= friction;
        posY++;
    } 

    else if (Hitbox::isPointWithin(rightPoint, otherHitbox))
    {
        // Handle right wall collision
        velX *= -1 * friction;
        posX--;
    }

    else if (Hitbox::isPointWithin(leftPoint, otherHitbox))
    {
        // Handle left wall collision
        velX *= -1 * friction;
        posX++;
    }

}