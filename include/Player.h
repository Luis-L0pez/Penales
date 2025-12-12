#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Ball.h"
#include "Power.h"

struct Player {
    std::string name;
    sf::Sprite sprite;
    float speed = 220.f;

    bool reversedControls = false;
    bool moveLeft = false;
    bool moveRight = false;

    Power currentPower;   // Power activo

    Player(const std::string& n);

    void update(float dt);
    sf::FloatRect getBounds() const;
    void kickBall(Ball& ball);
};
