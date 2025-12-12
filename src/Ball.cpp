#include "../include/Ball.h"
#include <cstdio>
#include <cmath>   // para std::sqrt


Ball::Ball() {
    static sf::Texture texture;
    if (!texture.loadFromFile("assets/ball.png")) {
        printf("No se pudo cargar ball.png\n");
    }
    sprite.setTexture(texture);
    sprite.setScale(0.05f, 0.05f);
    velocity = sf::Vector2f(0.f, 0.f);
}

void Ball::shoot(sf::Vector2f direction, float baseSpeed) {
    velocity = direction;
    float length = std::sqrt(direction.x*direction.x + direction.y*direction.y);
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
    // Resetear velocidad a la original si se desea
}

