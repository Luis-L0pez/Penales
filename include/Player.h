#pragma once
#include <SFML/Graphics.hpp>
#include <string>

// Forward declarations para romper la dependencia circular
struct Ball;
struct Power;   // Si solo usas por valor/reference/pointer, basta
struct Keeper;

struct Player {
    std::string name;
    sf::Sprite sprite;
    float speed = 220.f;

    bool reversedControls = false;
    bool moveLeft = false;
    bool moveRight = false;

    Power* currentPower = nullptr;   // Usar puntero evita problemas de forward declaration

    Player(const std::string& n);

    void update(float dt);
    sf::FloatRect getBounds() const;

    void kickBall(Ball& ball);  
};
