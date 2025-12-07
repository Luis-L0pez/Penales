#include "../include/ball.h"
#include <cmath>

void Ball::update(float dt) {
    // Mueve balón según velocidad
    sprite.move(velocity * dt);

    // Frena muy levemente si no está disparada
    if (!shot) {
        velocity *= 0.98f;
    }

    // Evita que el balón se vaya muy abajo (límite inferior)
    if (sprite.getPosition().y > 700) {
        sprite.setPosition(sprite.getPosition().x, 700);
        velocity.y = 0;
    }
}

void Ball::reset(const sf::Vector2f &pos) {
    sprite.setPosition(pos);
    velocity = {0.f, 0.f};
    shot = false;
}

sf::FloatRect Ball::getBounds() const {
    return sprite.getGlobalBounds();
}
