#include "Ball.h"
#include <iostream>
#include <cmath>

Ball::Ball() {
    if (!texture.loadFromFile("assets/ball.png")) {
        std::cerr << "Error: No se pudo cargar assets/ball.png\n";
        std::exit(EXIT_FAILURE);
    }

    sprite.setTexture(texture);

    // 🔧 BALÓN MÁS PEQUEÑO
    sprite.setScale(0.035f, 0.035f);

    velocity = sf::Vector2f(0.f, 0.f);
}

void Ball::shoot(sf::Vector2f direction, float baseSpeed) {
    velocity = direction;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0)
        velocity /= length;
    velocity *= baseSpeed;
}

void Ball::update(float dt) {
    sprite.move(velocity * dt);
    sprite.rotate(360.f * dt); // 🔥 
}