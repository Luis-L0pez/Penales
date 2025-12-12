#pragma once
#include <SFML/Graphics.hpp>
#include <string>

enum class PowerType {
    None = 0,
    SpeedBoost,
    Curve,
    FreezeKeeper,
    ReverseControls
};

struct Power {
    PowerType type = PowerType::None;
    std::string name() const;
};

struct Ball; // Declaración adelantada para poder usar Ball en kickBall

struct Player {
    std::string name;

    sf::Sprite sprite;
    float speed = 220.f;
    
    bool reversedControls = false;
    bool moveLeft = false;
    bool moveRight = false;

    Power currentPower;

    Player(const std::string& n);
    void update(float dt);
    sf::FloatRect getBounds() const;

    // Método para interactuar con la pelota
    void kickBall(Ball& ball);
};
