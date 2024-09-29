#include "Button.h"


Button::Button(const char* texturePath, float posX, float posY, float width, float height) : buttonRect({posX, posY, width, height})
{
    buttonTexture = LoadTexture(texturePath);
}

Button::~Button()
{
}

void Button::Draw() 
{
    Rectangle sourceRec = { 0.0f, 0.0f, (float)buttonTexture.width, (float)buttonTexture.height };
    DrawTexturePro(buttonTexture, sourceRec, buttonRect, { 0, 0 }, 0.0f, WHITE);
}

bool Button::isClicked(int mousePosX, int mousePosY) 
{
    if (mousePosX > buttonRect.x && mousePosX < buttonRect.x + buttonRect.width && mousePosY > buttonRect.y && mousePosY < buttonRect.y + buttonRect.height) {
        return true;
    }
    return false;
}