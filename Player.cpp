#include "Player.h"

Player::Player(float posX, float posY, int playerID) : 
    posX(posX), posY(posY), hitbox{posX, posY, 100, 100}, playerID(playerID)
{}

Player::~Player()
{
}

void Player::init(PlayerControls controls)
{
    this->controls = controls;
    jumpsLeft = numJumps;
}

void Player::HandleInput(PlayerCommand& playerCommand) 
{
    // If dead, do nothing
    if(isDead) return;

    // Walk left
    if (IsKeyDown(controls.moveLeft)) 
    {
        velX = -5.0f;
        playerFacing = PlayerFacing::left;
    }
    // Walk right
    else if (IsKeyDown(controls.moveRight)) 
    {
        velX = 5.0f;
        playerFacing = PlayerFacing::right;
    }
    else 
    {
        velX = 0.0f;
    }

    // Jump
    if(IsKeyPressed(controls.jump)) 
    {
        jump();
    }

    if(IsKeyPressed(controls.useWeapon))
    {
        // Todo: check which weapon is equiped
        throwGrenade(playerCommand);
    }
}

void Player::throwGrenade(PlayerCommand& playerCommand)
{
    // Command game class to deploy a grenade
    playerCommand.useWeapon = UseWeapon::Grenade;
    playerCommand.posX = this->posX;
    playerCommand.posY = this->posY;
    playerCommand.velX = this->velX;
    playerCommand.velY = this->velY;


    float grenadeSpeed = 3.0f;
    if (playerFacing == PlayerFacing::left)
    {
        playerCommand.velX -= grenadeSpeed;
    }
    else
    {
        playerCommand.velX += grenadeSpeed;
    }

}

void Player::HandleCollisions(Hitbox &otherHitbox)
{
    // Four points to check for collition
    FloatPoint topPoint = FloatPoint{posX + width / 2.0f, posY};
    FloatPoint bottomPoint = FloatPoint{posX + width / 2.0f, posY + height};
    FloatPoint leftPoint = FloatPoint{posX, posY + height / 2.0f};
    FloatPoint rightPoint = FloatPoint{posX + width, posY + height / 2.0f};

    if(Hitbox::isPointWithin(bottomPoint, otherHitbox))
    {
        // Handle floor collision
        velY = 0.0f;
        posY = otherHitbox.GetY() - height + 1;
        jumpsLeft = numJumps;
        onGround = true;
    }

    else if (Hitbox::isPointWithin(topPoint, otherHitbox)) 
    {
        // Handle ceiling collision
        velY = 0.0f;
        posY = otherHitbox.GetY() + otherHitbox.GetHeight() + 1;
    } 

    else if (Hitbox::isPointWithin(rightPoint, otherHitbox))
    {
        // Handle right wall collision
        velX = 0.0f;
        posX = otherHitbox.GetX() - width + 1;
    }

    else if (Hitbox::isPointWithin(leftPoint, otherHitbox))
    {
        // Handle left wall collision
        velX = 0.0f;
        posX = otherHitbox.GetX() + otherHitbox.GetWidth();
    }
}

void Player::jump()
{
    if (jumpsLeft > 0) 
    {
        posY -= 1;
        velY = -6.5f;
        jumpsLeft --;
        onGround = false;
    }
}

void Player::fall()
{
    float g = 0.3f;

    if (velY < maxFallVel) { 
        velY += g;
    }
}

void Player::Update() 
{ 
    fall();

    // Update hitbox position
    hitbox = Hitbox(posX, posY, width, height);

    if (health <= 0 && !isDead) { die(); }

    // Update pos from velovity
    posX += velX;
    posY += velY;
}

void Player::Draw(Texture2D& texture) 
{ 

    Rectangle sourceRec = getSourceRect(texture);
    Rectangle destRec = { posX, posY, width, height };

    // Tegn teksten
    DrawTexturePro(texture, sourceRec, destRec, { 0, 0 }, 0.0f, WHITE);
}

Rectangle Player::getSourceRect(Texture2D& texture)
{
    // Get current frame
    framesCounter++;

    // Update walking animation. Frame 2, 3 and 4
    if (framesCounter >= frameUpdateRate) {
        framesCounter = 0; 

        // Update only when player is moving and on ground
        if ((velX > 0 || velX < 0) && onGround) {
            if (currentFrame == 3) {
                currentFrame = 1;
            } else {
                currentFrame++;
            }
        } else {
            // Sett til stillestående ramme
            currentFrame = 0;
        }
    }

    int numFrames = 5;
    float currentFrameStartX = currentFrame * (float)texture.width / numFrames;
    float frameWidth = (float)texture.width / numFrames;

    if(isDead) 
    {
        int dethFrame = 4;
        return { (float)dethFrame * (float)texture.width / numFrames, 0.0f, frameWidth, (float)texture.height };
    }

    Rectangle sourceRec = { currentFrameStartX, 0.0f, frameWidth, (float)texture.height };
    // Sjekk hvilken retning spilleren vender
    if (playerFacing == PlayerFacing::left) {
        // Flipp teksten horisontalt
        sourceRec.width *= -1; // Gjør bredden negativ for å flippe
    }
    return sourceRec;
}

void Player::DrawHealthBar(float posX, float posY, float size, Color color)
{
    float barWhickness = 5.0f;
    DrawRectangle(posX, posY, size * health / 100, barWhickness, color);
}

void Player::die()
{
    isDead = true;
    posY--;
    velY = -5.0f;
}