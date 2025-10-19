#include "Ball.h"
#include "GameManager.h" // avoid cicular dependencies

Ball::Ball(sf::RenderWindow* window, float velocity, GameManager* gameManager)
    : _window(window), _velocity(velocity), _gameManager(gameManager),
    _timeWithPowerupEffect(0.f), _isFireBall(false), _isAlive(true), _direction({1,1})
{
    _sprite.setRadius(RADIUS);
    _sprite.setFillColor(sf::Color::Cyan);
    _sprite.setPosition(0, 300);

    _radius = RADIUS;
}

Ball::~Ball()
{
}

void Ball::update(float dt)
{
    // check for powerup, tick down or correct
    if (_timeWithPowerupEffect > 0.f)
    {
        _timeWithPowerupEffect -= dt;
    }
    else
    {
        if (_velocity != VELOCITY)
        {
            _velocity = VELOCITY;   // reset speed.
        }
        else if (_radius != RADIUS)
        {
            _radius = RADIUS;       // reset raidus
            _sprite.setRadius(_radius);
        }
        else
        {
            setFireBall(0);    // disable fireball
            _sprite.setFillColor(sf::Color::Cyan);  // back to normal colour.
        }        
    }

    // Fireball effect
    if (_isFireBall)
    {
        // Flickering effect
        int flicker = rand() % 50 + 205; // Random value between 205 and 255
        _sprite.setFillColor(sf::Color(flicker, flicker / 2, 0)); // Orange flickering color
    }

    // check bounds and bounce
    sf::Vector2f position = _sprite.getPosition();
    sf::Vector2u windowDimensions = _window->getSize();

    // Update position with a subtle floating-point error
    _sprite.move(_direction * _velocity * dt);

    updateTrail(dt);

    // bounce on walls
    if ((position.x >= windowDimensions.x - 2 * _radius && _direction.x > 0) || (position.x <= 0 && _direction.x < 0))
    {
        _direction.x *= -1;
        _gameManager->startScreenShake(0.2f, 4.0f);
    }

    // bounce on ceiling
    if (position.y <= 0 && _direction.y < 0)
    {
        _direction.y *= -1;
        _gameManager->startScreenShake(0.3f, 6.0f);
    }

    // collision with paddle
    if (_sprite.getGlobalBounds().intersects(_gameManager->getPaddle()->getBounds()))
    {
        _direction.y *= -1; // Bounce vertically

        float paddlePositionProportion = (_sprite.getPosition().x - _gameManager->getPaddle()->getBounds().left) / _gameManager->getPaddle()->getBounds().width;
        _direction.x = paddlePositionProportion * 2.0f - 1.0f;

        // Adjust position to avoid getting stuck inside the paddle
        _sprite.setPosition(_sprite.getPosition().x, _gameManager->getPaddle()->getBounds().top - 2 * _radius);
    }

    // collision with bricks
    int collisionResponse = _gameManager->getBrickManager()->checkCollision(_sprite, _direction);
    if (_isFireBall) return; // no collisisons when in fireBall mode.
    if (collisionResponse == 1)
    {
        _direction.x *= -1; // Bounce horizontally
    }
    else if (collisionResponse == 2)
    {
        _direction.y *= -1; // Bounce vertically
    }

    // lose life bounce
    if (position.y > windowDimensions.y)
    {
        _sprite.setPosition(0, 300);
        _direction = { 1, 1 };

        trailPos.clear();

        _gameManager->loseLife();
        
        return;
    }

}

void Ball::render()
{
    for (size_t i = 0; i < trailPos.size(); ++i)
    {
        float alphaFactor = 1.0f - static_cast<float>(i) / trailPos.size();
        sf::Uint8 alpha = static_cast<sf::Uint8>(alphaFactor * 255);

        sf::CircleShape trailDot(_radius * (0.75f + 0.25f * alphaFactor));
        trailDot.setPosition(trailPos[i]);

        if (_isFireBall)
        {
            trailDot.setFillColor(sf::Color(255, 100, 0, alpha));
        }
        else
        {
            trailDot.setFillColor(sf::Color(0, 255, 255, alpha));
        }

        _window->draw(trailDot);
    }

    _window->draw(_sprite);
}

void Ball::setVelocity(float coeff, float duration)
{
    _velocity = coeff * VELOCITY;
    _timeWithPowerupEffect = duration;
}

void Ball::setFireBall(float duration)
{
    if (duration) 
    {
        _isFireBall = true;
        _timeWithPowerupEffect = duration;        
        return;
    }
    _isFireBall = false;
    _timeWithPowerupEffect = 0.f;    
}

void Ball::setRadius(float coeff, float duration)
{
    _radius = coeff * RADIUS;
    _sprite.setRadius(_radius);
    _timeWithPowerupEffect = duration;
}

void Ball::updateTrail(float dt)
{
    //slows the removal of trail
    trailTimer += dt;
    if (trailTimer >= trailUpdateInterval)
    {
        //add current ball pos to the front
        trailPos.push_front(_sprite.getPosition());

        //limit the amount of balls in trail
        if (trailPos.size() > trailLength)
        {
            trailPos.pop_back();
        }

        trailTimer = 0.f;
    }
}
