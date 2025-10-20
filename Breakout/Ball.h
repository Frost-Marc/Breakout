#pragma once
#include <SFML/Graphics.hpp>
#include <deque>


class GameManager;  // forward declaration



class Ball {
public:
    Ball(sf::RenderWindow* window, float velocity, GameManager* gameManager);
    ~Ball();
    void update(float dt);
    void render();
    void setVelocity(float coeff, float duration);
    void setFireBall(float duration);
    void setRadius(float coeff, float duration);

private:
    sf::CircleShape _sprite;
    sf::Vector2f _direction;
    sf::RenderWindow* _window;
    float _velocity;
    float _radius;
    bool _isAlive;
    bool _isFireBall;
    float _timeWithPowerupEffect;

    GameManager* _gameManager;  // Reference to the GameManager


    static constexpr float RADIUS = 10.0f;      
    static constexpr float VELOCITY = 350.0f;   // for reference.

    std::deque<sf::Vector2f> trailPos;
    const int trailLength = 100;
    float trailTimer = 0.f;
    const float trailUpdateInterval = 0.015f;
    sf::Color trailColour = sf::Color(0, 255, 255, 255);

    void updateTrail(float dt);
};

