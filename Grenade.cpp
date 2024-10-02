#include "Grenade.h"


Grenade::Grenade(float posX, float posY, float velX, float velY, int throwerID) : 
    posX(posX), posY(posY), velX(velX), velY(velY), hitbox(posX, posY, size, size), throwerID(throwerID)
{
    
}

Grenade::~Grenade()
{
    // Todo: find out how to unload textures for multiple instances of
    // the same texture. Texture manager class?
}



void Grenade::Draw(Texture2D& texture)    
{
    Rectangle sourceRec = getSourceRect(texture);
    Rectangle destRec = { posX, posY, size, size };
    
    if(exploded)
    {
        // increase size of destRec
        float multiplier = 10.0f;
        destRec = { posX - (size * multiplier / 2.0f), posY - (size * multiplier / 2.0f), size * multiplier, size * multiplier };
    }

    DrawTexturePro(texture, sourceRec, destRec, { 0, 0 }, 0.0f, WHITE);
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

    if(exploded)
    {
        numFramesForExplotion--;
        if(numFramesForExplotion <= 0) { isDead = true; }
    }
}

void Grenade::explode()
{
    velX = 0;
    velY = 0;
    exploded = true;
}

Rectangle Grenade::getSourceRect(Texture2D& texture)
{   
    int frameUpdateRate = 3;
    bool isMirrored = false;
    if (exploded)
    {
        return getAnimatedSourceRect(1, 4, texture, isMirrored, frameUpdateRate);
    }
    else
    {
        return getAnimatedSourceRect(0, 0, texture, isMirrored);
    }
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
    // Handle floor collision
    if(Hitbox::isPointWithin(bottomPoint, otherHitbox) && !Hitbox::isPointWithin(topPoint, otherHitbox))
    {
        velY *= -1 * friction;
        velX *= friction;
        posY = otherHitbox.GetY() - size;
    }

    // Handle ceiling collision
    else if (Hitbox::isPointWithin(topPoint, otherHitbox) && !Hitbox::isPointWithin(bottomPoint, otherHitbox)) 
    {
        velY *= -1 * friction;
        velX *= friction;
        posY = otherHitbox.GetY() + otherHitbox.GetHeight();
    } 

    // Handle right wall collision
    else if (Hitbox::isPointWithin(rightPoint, otherHitbox) && velX > 0)
    {
        velX *= -1 * friction;
        posX = otherHitbox.GetX() - size;
    }

    // Handle left wall collision
    else if (Hitbox::isPointWithin(leftPoint, otherHitbox) && velX < 0)
    {
        velX *= -1 * friction;
        posX = otherHitbox.GetX() + otherHitbox.GetWidth();
    }

}