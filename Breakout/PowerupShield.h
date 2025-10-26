#pragma once
#include "PowerupBase.h"

class PowerupShield : public PowerupBase
{
public:
	PowerupShield(sf::RenderWindow* window, Paddle* paddle, Ball* ball);

	std::pair<POWERUPS, float> applyEffect() override; //override function so all powerups can be applied using the same fucntion
};

