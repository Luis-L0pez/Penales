#include "../include/Player.h"
#include "../include/Ball.h"
#include "../include/Power.h"
#include <cstdio>

Player::Player(const std::string &name) {
    speed = 220.f;
    currentPower.type = PowerType::NONE;
    currentPower.active = false;

    static sf::Texture texture;
    if (!texture.loadFromFile("assets/player.png")) {
        printf("No se pudo cargar player.png\n");
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
    if (pos.x < 0.f) sprite.setPosition(0.f, pos.y);
    if (pos.x > 800.f - sprite.getGlobalBounds().width)
        sprite.setPosition(800.f - sprite.getGlobalBounds().width, pos.y);
}

sf::FloatRect Player::getBounds() const {
    return sprite.getGlobalBounds();
}

void Player::kickBall(Ball &ball) {
    sf::Vector2f direction(0.f, -1.f);
    float baseSpeed = ball.speed;

    if (currentPower.type == PowerType::SPEED_BOOST)
        ball.activateSpeedBoost(1.5f);
    else
        ball.deactivatePowerUps();

    ball.shoot(direction, baseSpeed);

    currentPower.type = PowerType::NONE;
    currentPower.active = false;
}
