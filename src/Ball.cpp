#include "../include/Ball.h"

Ball::Ball() {
    velocity = {0.f, 0.f};
    isMoving = false;
    speedBoost = false;
    boostMultiplier = 1.5f;
}

void Ball::shoot(const sf::Vector2f& direction, float baseSpeed) {
    float finalSpeed = baseSpeed;

    // Aplica power-up si está activo
    if (speedBoost) {
        finalSpeed *= boostMultiplier;
    }

    velocity = direction * finalSpeed;
    isMoving = true;
}

void Ball::update(float dt) {
    if (isMoving) {
        sprite.move(velocity * dt);

        // Detener la pelota si sale de pantalla
        sf::Vector2f pos = sprite.getPosition();
        if (pos.y < -50 /*|| pos.y > windowHeight || pos.x < -50 || pos.x > windowWidth*/) {
            velocity = {0.f, 0.f};
            isMoving = false;

            // Reinicia power-ups al detenerse
            speedBoost = false;
        }
    }
}

sf::FloatRect Ball::getBounds() const {
    return sprite.getGlobalBounds();
}

// Métodos para manejar power-ups
void Ball::activateSpeedBoost(float multiplier) {
    speedBoost = true;
    boostMultiplier = multiplier;
}

void Ball::deactivatePowerUps() {
    speedBoost = false;
}
