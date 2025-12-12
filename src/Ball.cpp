#include "Ball.h"
#include <iostream>
#include <cmath>

Ball::Ball() {
    if (!texture.loadFromFile("assets/ball.png")) {
        std::cerr << "Error: No se pudo cargar assets/ball.png\n";
        std::exit(EXIT_FAILURE);
    }

    sprite.setTexture(texture);
    sprite.setScale(0.05f, 0.05f);
    velocity = sf::Vector2f(0.f, 0.f);
}

void Ball::shoot(sf::Vector2f direction, float baseSpeed) {
    float length = std::sqrt(direction.x * direction.x +
                             direction.y * direction.y);

    if (length != 0.f)
        direction /= length;

    velocity = direction * baseSpeed;
}

void Ball::update(float dt) {
    sprite.move(velocity * dt);
}