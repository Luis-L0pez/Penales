#include "../include/Player.h"
#include "../include/Ball.h"  // Para interactuar con la pelota
#include <cstdio>

Player::Player(const std::string& n) : name(n) {
    speed = 220.f;
    reversedControls = false;
    currentPower.type = PowerType::None;

    static sf::Texture texture;
    if (!texture.loadFromFile("assets/player.png")) {
        printf("No se pudo cargar player.png\n");
    }
    sprite.setTexture(texture);

    // Ajusta tamaño del sprite si está muy grande
    sprite.setScale(0.5f, 0.5f);

    // Posición inicial
    sprite.setPosition(200.f, 400.f);
}

void Player::update(float dt) {
    float movement = 0.f;

    if (moveLeft)  movement -= speed;
    if (moveRight) movement += speed;

    if (reversedControls)
        movement *= -1;

    sprite.move(movement * dt, 0.f);

    // Limitar al jugador dentro de la pantalla (ejemplo 0 a 800 px)
    sf::Vector2f pos = sprite.getPosition();
    if (pos.x < 0.f) sprite.setPosition(0.f, pos.y);
    if (pos.x > 800.f - sprite.getGlobalBounds().width)
        sprite.setPosition(800.f - sprite.getGlobalBounds().width, pos.y);
}

sf::FloatRect Player::getBounds() const {
    return sprite.getGlobalBounds();
}

// Función para patear la pelota aplicando power-ups
void Player::kickBall(Ball& ball) {
    // Dirección hacia arriba (puedes ajustar según tu juego)
    sf::Vector2f direction(0.f, -1.f);

    float baseSpeed = ball.speed;

    // Aplicar power-ups
    if (currentPower.type == PowerType::SpeedBoost) {
        ball.activateSpeedBoost(1.5f);  // Multiplicador
    } else {
        ball.deactivatePowerUps();
    }

    // Dispara la pelota
    ball.shoot(direction, baseSpeed);

    // Una vez usado, quitar el power-up
    currentPower.type = PowerType::None;
}
