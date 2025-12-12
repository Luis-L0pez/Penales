#pragma once
#include <SFML/Graphics.hpp>

class Keeper {
public:
    sf::Sprite sprite;
    float speed;

    Keeper();
    void update(float dt);
};