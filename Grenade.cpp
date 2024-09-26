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
}

void Grenade::explode()
{
    velX = 0;
    velY = 0;
    exploded = true;
}

Rectangle Grenade::getSourceRect(Texture2D& texture)
{   
    if(exploded) { framesCounter++; }

    int frameUpdateRate = 3;           // Hvor raskt animasjonen oppdateres
    int animationEndFrame = 4;         // Siste rammenummer for animasjonen (ikke inkludert)
    int numFrames = 5;                 // Totalt antall rammer i sprite-arket

    // Oppdater animasjonsrammen
    if (framesCounter >= frameUpdateRate) 
    {
        framesCounter = 0; 
        currentFrame++;

        if (currentFrame >= animationEndFrame)
        {
            isDead = true;
        }
    }

    // Beregn bredden og startpunktet for den nåværende animasjonsrammen
    float frameWidth = (float)texture.width / numFrames;
    float currentFrameStartX = currentFrame * frameWidth;

    // Returner rektangelet for den nåværende animasjonsrammen
    Rectangle sourceRec = { currentFrameStartX, 0.0f, frameWidth, (float)texture.height };
    
    return sourceRec;
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