#pragma once
#include <SFML/Graphics.hpp>

struct Ball {
    sf::Sprite sprite;
    sf::Vector2f velocity = {0.f, 0.f};   // ✔ Ahora sí está definido

    bool isMoving = false;                // Saber si ya fue pateado

    void update(float dt);
    void shoot(const sf::Vector2f& direction, float speed);  // ✔ Declarado aquí
    sf::FloatRect getBounds() const;
};
