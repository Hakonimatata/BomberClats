#pragma once
#include <raylib.h>

class Button
{
    public:
        Button() = default;
        Button(const char* texturePath, float posX, float posY, float width, float height);
        ~Button();

        void Draw();
        bool isClicked(int mousePosX, int mousePosY);

    private:
        Rectangle buttonRect;
        // float posX, posY, width, height;
        Texture2D buttonTexture;
};