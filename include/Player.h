#pragma once
#include <SFML/Graphics.hpp>
#include <string>

// Forward declarations
struct Ball;
struct Power;
struct Keeper;

struct Player {
    std::string name;
    sf::Sprite sprite;
    float speed = 220.f;

    bool reversedControls = false;
    bool moveLeft = false;
    bool moveRight = false;

    Power* currentPower = nullptr;  // puntero

    Player(const std::string& n);

    void update(float dt);
    sf::FloatRect getBounds() const;

    void kickBall(Ball& ball);
};
