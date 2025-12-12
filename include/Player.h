#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include "Power.h"  // Solo incluir, no redefinir nada

struct Player {
    std::string name;
    sf::Sprite sprite;
    float speed = 220.f;

    bool reversedControls = false;
    bool moveLeft = false;
    bool moveRight = false;

    Power currentPower; // si necesitas tenerlo aquí

    Player(const std::string& n);
    void update(float dt);
    sf::FloatRect getBounds() const;
};
