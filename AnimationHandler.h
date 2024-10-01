#pragma once

#include <raylib.h>

class AnimationHandler
{
    public:

        AnimationHandler() = default;

    protected:

        // Animation
        int currentFrame = 0;
        int framesCounter = 0;

        // Generates animated source rectangle
        Rectangle getAnimatedSourceRect(int frameStart, int frameEnd, Texture2D& texture, bool mirror, int frameUpdateRate = 5);

};