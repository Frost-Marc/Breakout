#include "PowerupShield.h"

PowerupShield::PowerupShield(sf::RenderWindow* window, Paddle* paddle, Ball* ball) : PowerupBase (window, paddle, ball)
{
	_sprite.setFillColor(otherEffectsColour);
}

std::pair<POWERUPS, float> PowerupShield::applyEffect()
{
	_ball->activateShield(); // function created in ball class for setting the shield effect active

	return { POWERUPS::shield, 0.f }; //duration set to 0 since it is one use instead of on a timer
}
