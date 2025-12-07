#pragma once
#include <SFML/Graphics.hpp>

struct Ball {
    sf::Sprite sprite;
    sf::Vector2f velocity{0,0};
    bool shot = false;

    void update(float dt);
    void reset(const sf::Vector2f &pos);
    sf::FloatRect getBounds() const;
};
