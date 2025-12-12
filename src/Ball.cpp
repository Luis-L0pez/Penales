#include "../include/Ball.h"
#include <cstdio>

Ball::Ball() {
    speed = 500.f;
    static sf::Texture texture;
    if (!texture.loadFromFile("assets/ball.png"))
        printf("No se pudo cargar ball.png\n");
    sprite.setTexture(texture);
    sprite.setScale(0.05f, 0.05f);
    velocity = sf::Vector2f(0.f, 0.f);
}

void Ball::update(float dt) {
    sprite.move(velocity * dt);
}

void Ball::shoot(sf::Vector2f direction, float baseSpeed) {
    velocity = direction;
    velocity.x *= baseSpeed;
    velocity.y *= baseSpeed;
}

void Ball::activateSpeedBoost(float factor) {
    velocity *= factor;
}

void Ball::deactivatePowerUps() {
    // reset si quieres
}
