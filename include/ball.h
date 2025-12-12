#pragma once
#include <SFML/Graphics.hpp>

struct Ball {
    sf::Sprite sprite;
    sf::Vector2f velocity;
    float speed = 500.f;

    Ball();  // Constructor
    void shoot(sf::Vector2f dir, float spd);
    void update(float dt);

    sf::FloatRect getBounds() const;
};
