#pragma once
#include <SFML/Graphics.hpp>
#include <string>

struct Power; // forward declaration
class Ball;

class Player {
public:
    sf::Sprite sprite;
    float speed;
    bool moveLeft = false;
    bool moveRight = false;
    bool reversedControls = false;
    Power currentPower;

    Player(const std::string &name);

    void update(float dt);
    sf::FloatRect getBounds() const;
    void kickBall(Ball &ball);
};

