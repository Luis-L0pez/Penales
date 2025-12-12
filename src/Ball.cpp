#include "../include/Ball.h"

void Ball::shoot(const sf::Vector2f& direction, float speed) {
    velocity = direction * speed;   // ✔ Aplica velocidad
    isMoving = true;
}

void Ball::update(float dt) {
    if (isMoving) {
        sprite.move(velocity * dt);

        // Detener la pelota si sale de pantalla
        if (sprite.getPosition().y < -50) {
            velocity = {0.f, 0.f};
            isMoving = false;
        }
    }
}

sf::FloatRect Ball::getBounds() const {
    return sprite.getGlobalBounds();
}
