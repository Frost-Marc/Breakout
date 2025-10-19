#pragma once

#include "PowerupManager.h"

class PowerupBigBall : public PowerupBase
{
public:
    PowerupBigBall(sf::RenderWindow* window, Paddle* paddle, Ball* ball);
    ~PowerupBigBall();

    std::pair<POWERUPS, float> applyEffect() override; // Method to apply the power-up effect
};

