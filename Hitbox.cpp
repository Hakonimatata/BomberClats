#include "Hitbox.h"

// Constructor that initializes the hitbox with specific dimensions
Hitbox::Hitbox(float x, float y, float width, float height) {
    hitboxRect.x = x;
    hitboxRect.y = y;
    hitboxRect.width = width;
    hitboxRect.height = height;
}

// Constructor that initializes the hitbox from an SDL_Rect
Hitbox::Hitbox(const Rectangle& rect) {
    hitboxRect = rect;
}

// Check for collision with another hitbox
bool Hitbox::CheckCollision(const Hitbox& other) const {

    return CheckCollisionRecs(hitboxRect, other.hitboxRect);
}

// Scale the hitbox by a factor
void Hitbox::SetScale(float scaleFactor) {
    hitboxRect.width = (hitboxRect.width * scaleFactor);
    hitboxRect.height = (hitboxRect.height * scaleFactor);
    hitboxRect.x = (hitboxRect.x * scaleFactor);
    hitboxRect.y = (hitboxRect.y * scaleFactor);
}

bool Hitbox::isPointWithin(FloatPoint& point, Hitbox& hitbox)
{
    if(point.x >= hitbox.GetX() && point.x <= hitbox.GetX() + hitbox.GetWidth() 
        && point.y >= hitbox.GetY() && point.y <= hitbox.GetY() + hitbox.GetHeight())
    {
        return true;
    }
    else
    {
        return false;
    }
}
