#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Ball.h"
#include "Power.h"

class Player {
public:
    sf::Sprite sprite;
    float speed;
    bool moveLeft = false;
    bool moveRight = false;
    bool reversedControls = false;
    Power currentPower;
    std::string name;

    Player(const std::string &n);

    void update(float dt);
    void kickBall(Ball &ball);
    sf::FloatRect getBounds() const;
};
