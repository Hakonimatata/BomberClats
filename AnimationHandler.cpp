#include "AnimationHandler.h"

Rectangle AnimationHandler::getAnimatedSourceRect(int frameStart, int frameEnd, Texture2D &texture, bool mirror, int frameUpdateRate)
{
    // update frames counter
    framesCounter++;
    if (framesCounter >= frameUpdateRate) {
        // Reset frames counter
        framesCounter = 0; 

        // Generate next frame
        currentFrame = (currentFrame + 1) % (frameEnd - frameStart + 1) + frameStart;
    }

    int numFrames = texture.width / texture.height;
    float currentFrameStartX = currentFrame * (float)texture.width / numFrames;
    float frameWidth = (float)texture.width / numFrames;

    Rectangle sourceRec = { currentFrameStartX, 0.0f, frameWidth, (float)texture.height };

    if (mirror) {
        sourceRec.width *= -1;
    }

    return sourceRec;
}