#pragma once
#include <SFML/Graphics.hpp>

class Ball {
public:
    sf::Sprite sprite;
    float speed = 400.f;

    Ball();
    void shoot(sf::Vector2f direction, float baseSpeed);
    void update(float dt);
    void activateSpeedBoost(float factor);
    void deactivatePowerUps();

private:
    sf::Vector2f velocity;
};