#pragma once
#include <SFML/Graphics.hpp>

class Ball {
public:
    sf::Sprite sprite;
    sf::Texture texture;     // ✅ LA TEXTURA DEBE VIVIR AQUÍ
    sf::Vector2f velocity;

    Ball();
    void shoot(sf::Vector2f direction, float baseSpeed);
    void update(float dt);
};