#pragma once
#include <SFML/Graphics.hpp>

class Ball {
public:
    sf::Sprite sprite;
    sf::Vector2f velocity;
    float speed;

    Ball();

    void update(float dt);
    void shoot(sf::Vector2f direction, float baseSpeed);
    void activateSpeedBoost(float factor);
    void deactivatePowerUps();
};
