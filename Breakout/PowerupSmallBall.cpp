#include "PowerupSmallBall.h"

PowerupSmallBall::PowerupSmallBall(sf::RenderWindow* window, Paddle* paddle, Ball* ball) : PowerupBase(window, paddle, ball)
{
	_sprite.setFillColor(ballEffectsColour);
}

PowerupSmallBall::~PowerupSmallBall()
{
}

std::pair<POWERUPS, float> PowerupSmallBall::applyEffect()
{
	_ball->setRadius(0.5f, 5.0f);
	return { smallBall, 5.0f };
}
