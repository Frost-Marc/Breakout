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

        trailColour = sf::Color(rand() % 255, rand() % 255, rand() % 255);
    }
    else if (collisionResponse == 2)
    {
        _direction.y *= -1; // Bounce vertically

        trailColour = sf::Color(rand() % 255, rand() % 255, rand() % 255);
    }

    // lose life bounce
    if (position.y > windowDimensions.y)
    {
        if (_shieldActive)
        {
            _direction.y *= -1; //bounces ball off shield if it is active
            _sprite.setPosition(_sprite.getPosition().x, windowDimensions.y - 50.f); //pushes the ball sprite above the shield
            _shieldActive = false; //shield is one time use each pick up
            return; //returns so the lose of life code is not run while the shield is active
        }

        _sprite.setPosition(0, 300);
        _direction = { 1, 1 };

        trailDots.clear();

        _gameManager->loseLife();
        
        return;
    }

}

void Ball::render()
{
    //renders each circle
    for (size_t i = 0; i < trailDots.size(); ++i)
    {
        //changes the alpha value based on how many circles are in the trail
        float alphaFactor = 1.0f - static_cast<float>(i) / trailDots.size();
        sf::Uint8 alpha = static_cast<sf::Uint8>(alphaFactor * 255);

        //creates the circle setting its radius and position
        sf::CircleShape trailDot(_radius * (0.75f + 0.25f * alphaFactor));
        trailDot.setPosition(trailDots[i].Position);

        //sets the colour and alpha of the circle as it is rendered
        sf::Color c = trailDots[i].Colour;
        c.a = alpha;
        trailDot.setFillColor(c);

        _window->draw(trailDot);
    }

    _window->draw(_sprite);

    if (_shieldActive)
    {
        _window->draw(_shield); //only draws shield while it is active
    }
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

void Ball::activateShield()
{
    _shieldActive = true;
    _shield.setSize(sf::Vector2f(_window->getSize().x, 10.f)); //sets shield width and height
    _shield.setFillColor(sf::Color(255, 165, 0)); //sets shield colour
    _shield.setPosition((_window->getSize().x - _shield.getSize().x) / 2, _window->getSize().y - 10.f); //shield position is set to just above the bottom of the screen
}

void Ball::updateTrail(float dt)
{
    //slows the removal of trail
    trailTimer += dt;

    //whenever the ball moves updates the trail to add or remove circles from the trail
    if (trailTimer >= trailUpdateInterval)
    {
        TrailDot dot;

        dot.Position = _sprite.getPosition();
        
        //updates the colour of the circles as they are made to allow for mid trail changes
        if (_isFireBall)
        {
            dot.Colour = sf::Color(255, 100, 0);
        }
        else
        {
            dot.Colour = trailColour;
        }

        trailDots.push_front(dot);

        //limit the amount of balls in trail
        if (trailDots.size() > trailLength)
        {
            trailDots.pop_back();
        }

        trailTimer = 0.f;
    }
}
