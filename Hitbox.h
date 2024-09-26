#pragma once

#include <string>
#include <raylib.h>
#include "Utilities.h"


class Hitbox {
public:
    // Constructor
    Hitbox(float x, float y, float width, float height);
    Hitbox(const Rectangle& rect);

    // Check for collision with another hitbox
    bool CheckCollision(const Hitbox& other) const;

    // Scale the hitbox
    void SetScale(float scaleFactor);

    // Getters
    int GetX() const { return hitboxRect.x; }
    int GetY() const { return hitboxRect.y; }
    int GetWidth() const { return hitboxRect.width; }
    int GetHeight() const { return hitboxRect.height; }

    // Get the SDL_Rect for rendering or other purposes
    const Rectangle& GetRect() const { return hitboxRect; }


    static bool isPointWithin(FloatPoint& point, Hitbox& hitbox);

private:
    Rectangle hitboxRect; // Rectangle representing the hitbox
};