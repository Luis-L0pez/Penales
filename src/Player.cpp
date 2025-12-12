#include "Player.h"
#include "ball.h"
#include <iostream>

Player::Player(const std::string &name) {
    speed = 220.f;

    static sf::Texture texture;
    if (!texture.loadFromFile("assets/player.png")) {
        std::cerr << "Error: No se pudo cargar assets/player.png. Saliendo...\n";
        exit(-1);
    }
    sprite.setTexture(texture);
    sprite.setScale(0.5f, 0.5f);
    sprite.setPosition(200.f, 400.f);
}

void Player::update(float dt) {
    float movement = 0.f;
    if (moveLeft) movement -= speed;
    if (moveRight) movement += speed;
    if (reversedControls) movement *= -1;
    sprite.move(movement * dt, 0.f);

    sf::Vector2f pos = sprite.getPosition();
    sf::FloatRect bounds = sprite.getGlobalBounds();
    if (bounds.width > 0.f) {
        if (pos.x < 0.f) sprite.setPosition(0.f, pos.y);
        if (pos.x > 800.f - bounds.width) sprite.setPosition(800.f - bounds.width, pos.y);
    }
}

sf::FloatRect Player::getBounds() const {
    return sprite.getGlobalBounds();
}

void Player::kickBall(Ball &ball) {
    sf::Vector2f direction(0.f, -1.f);
    float baseSpeed = 500.f;
    ball.shoot(direction, baseSpeed);
}