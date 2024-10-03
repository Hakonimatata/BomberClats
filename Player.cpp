#include "Player.h"

Player::Player(float posX, float posY, int playerID) : 
    posX(posX), posY(posY), spawnPosX(posX), spawnPosY(posY), hitbox{posX, posY, 0, 0}, playerID(playerID)
{
    hitbox = Hitbox(posX, posY, width, height);
}

Player::~Player()
{
}

void Player::init(PlayerControls controls)
{
    this->controls = controls;
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

    if (selectedWeapon == SelectedWeapon::Grenade)
    {
        //  Use is key down for grenade, so throw velocity can be charged up
        if (IsKeyDown(controls.useWeapon) && grenadeThrowTimer <= 0.0f)
        {
            if (grenadeCharge < maxGrenadeCharge) { grenadeCharge += 0.1f; }
        }
        else if (grenadeCharge > 0.0f) { throwGrenade(playerCommand); }
    }
}

void Player::throwGrenade(PlayerCommand& playerCommand)
{
    if (grenadeThrowTimer > 0.0f) return;
    else { grenadeThrowTimer = grenadeThrowDelay; }

    // Command game class to deploy a grenade
    playerCommand.useWeapon = UseWeapon::Grenade;
    playerCommand.posX = this->posX;
    playerCommand.posY = this->posY;
    playerCommand.velX = this->velX;
    playerCommand.velY = this->velY;


    float grenadeSpeed = 2.0f + grenadeCharge;
    if (playerFacing == PlayerFacing::left)
    {
        playerCommand.velX -= grenadeSpeed;
    }
    else
    {
        playerCommand.velX += grenadeSpeed;
    }
    playerCommand.velY -= 5 - grenadeCharge / 2;
    grenadeCharge = 0.0f;
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
        posX = otherHitbox.GetX() + otherHitbox.GetWidth() - 1;
    }
}

void Player::Reset()
{
    health = 100.0f;
    isDead = false;
    posX = spawnPosX;
    posY = spawnPosY;
    velX = 0.0f;
    velY = 0.0f;
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

void Player::Update(float deltaTime) 
{ 
    fall();

    // Update grenade throw timer
    if (grenadeThrowTimer > 0.0f) { grenadeThrowTimer -= deltaTime; }

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

void Player::DrawCrown(Texture2D &texture)
{
    DrawTexturePro(texture, { 0, 0, (float)texture.width, (float)texture.height }, { posX, posY - height / 2, width, height }, { 0, 0 }, 0.0f, WHITE);
}

void Player::DrawScore(int score)
{
    DrawText(to_string(score).c_str(), posX, posY - 15, 20, WHITE);

}

Rectangle Player::getSourceRect(Texture2D& texture)
{
    bool isMirrored = false;
    if(playerFacing == PlayerFacing::left){ isMirrored = true; }

    if ((velX > 0 || velX < 0) && onGround)  // Running
    {
        return getAnimatedSourceRect(1, 3, texture, isMirrored);
    }
    else if (isDead) // Dead
    {
        return getAnimatedSourceRect(4, 4, texture, isMirrored);
    }
    else if (!onGround && velX != 0) // Jumping
    {
        return getAnimatedSourceRect(5, 5, texture, isMirrored);
    }
    else // Idle
    {
        return getAnimatedSourceRect(0, 0, texture, isMirrored);
    }
}

void Player::DrawStatus()
{
    // Draw helth status
    float barWhickness = 5.0f;
    DrawRectangle(posX, posY - 20, width * health / 100, barWhickness, RED);
    DrawRectangleLines(posX, posY - 20, width, barWhickness, BLACK);

    // Draw grenade timeout status
    if(grenadeThrowTimer > 0.0f) 
    {
       DrawRectangle(posX, posY - 20 + barWhickness, width * grenadeThrowTimer / grenadeThrowDelay, barWhickness, WHITE);
    }

    // Draw grenade charge
    if(grenadeCharge > 0.0f) 
    {
       DrawRectangle(posX, posY - 20 + barWhickness, width * grenadeCharge / maxGrenadeCharge, barWhickness, YELLOW);
    }
}

void Player::die()
{
    isDead = true;
    posY--;
    velY = -5.0f;
}