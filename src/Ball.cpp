#include "Ball.h"
#include <iostream>
#include <cmath>

Ball::Ball() {
    if (!texture.loadFromFile("assets/ball.png")) {
        std::cerr << "Error cargando ball.png\n";
        std::exit(EXIT_FAILURE);
    }
    sprite.setTexture(texture);
    sprite.setScale(0.035f, 0.035f);
    velocity = sf::Vector2f(0.f, 0.f);
}

void Ball::shoot(sf::Vector2f direction, float baseSpeed) {
    velocity = direction;
    float len = std::sqrt(direction.x*direction.x + direction.y*direction.y);
    if (len != 0) velocity /= len;
    velocity *= baseSpeed;
}

void Ball::update(float dt) {
    sprite.move(velocity * dt);
    sprite.rotate(360.f * dt); // animación visible
}