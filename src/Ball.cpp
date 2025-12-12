#include "Ball.h"
#include <iostream>
#include <cmath>

Ball::Ball() {
    static sf::Texture texture;
    if (!texture.loadFromFile("assets/ball.png")) {
        std::cerr << "Error: No se pudo cargar assets/ball.png. Saliendo...\n";
        exit(-1);
    }
    sprite.setTexture(texture);
    sprite.setScale(0.05f, 0.05f);
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
}

void Ball::activateSpeedBoost(float factor) {
    velocity *= factor;
}

void Ball::deactivatePowerUps() {
    // Vacío por ahora
}
void Ball::deactivatePowerUps() {
    // Resetear velocidad a la original si se desea
}

