#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Player {
public:
    sf::Sprite sprite;
    sf::Texture texture;     // ✅ textura como miembro
    float speed = 220.f;

    bool moveLeft = false;
    bool moveRight = false;

    Player(const std::string& name);
    void update(float dt);

private:
    std::string name;
};
