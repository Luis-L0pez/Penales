#pragma once
#include <SFML/Graphics.hpp>

struct Keeper {
    sf::Sprite sprite;
    float speed = 200.f;
    int direction = 0;

    bool frozen = false;
    float freezeDuration = 1.f;
    sf::Clock freezeClock;

    void update(float dt);
    sf::FloatRect getBounds() const;
};
