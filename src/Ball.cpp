#include "Ball.h"
#include <SFML/Graphics.hpp>
#include <cstdio>

Ball::Ball() {
    static sf::Texture texture;
    if (!texture.loadFromFile("assets/ball.png")) {
        printf("No se pudo cargar ball.png\n");
    }
    sprite.setTexture(texture);
    sprite.setScale(0.5f, 0.5f);
    velocity = sf::Vector2f(0.f, 0.f);
}

// Dispara la pelota en una dirección con cierta velocidad
void Ball::shoot(const sf::Vector2f& direction, float s) {
    velocity = direction;
    speed = s;
}

// Actualiza la posición de la pelota
void Ball::update(float dt) {
    sprite.move(velocity * dt);

    // Ejemplo: si quieres que frene con el tiempo
    // velocity *= 0.99f;
}

// Métodos para power-ups
void Ball::activateSpeedBoost(float multiplier) {
    speed *= multiplier;
    velocity *= multiplier; // aplica al vector de movimiento también
}

void Ball::deactivatePowerUps() {
    speed = 500.f;        // valor base
    // Opcional: normaliza la velocidad si quieres
}
