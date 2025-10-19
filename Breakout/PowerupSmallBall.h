#pragma once

#include "PowerupManager.h"

class PowerupSmallBall : public PowerupBase
{
public:
    PowerupSmallBall(sf::RenderWindow* window, Paddle* paddle, Ball* ball);
    ~PowerupSmallBall();

    std::pair<POWERUPS, float> applyEffect() override; // Method to apply the power-up effect
};
