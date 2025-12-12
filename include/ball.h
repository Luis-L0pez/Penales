#pragma once
#include <SFML/Graphics.hpp>

struct Ball {
    sf::Sprite sprite;
    sf::Vector2f velocity = {0.f, 0.f};
    bool isMoving = false;
    float speed = 300.f; // <-- Agregado

    void update(float dt);
    void shoot(const sf::Vector2f& direction, float speed);
    sf::FloatRect getBounds() const;

    // Power-ups (opcional si los quieres)
    bool speedBoost = false;
    float boostMultiplier = 1.5f;

    void activateSpeedBoost(float multiplier = 1.5f);
    void deactivatePowerUps();
};
