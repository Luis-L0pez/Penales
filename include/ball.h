#pragma once
#include <SFML/Graphics.hpp>

struct Ball {
    sf::Sprite sprite;
    sf::Vector2f velocity;
    float speed = 500.f;

    // Constructor
    Ball();

    // Movimiento y actualización
    void shoot(const sf::Vector2f& direction, float s);
    void update(float dt);

    // Métodos para power-ups
    void activateSpeedBoost(float multiplier);
    void deactivatePowerUps();
};
