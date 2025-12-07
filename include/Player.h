
#include "Player.h"


Player::Player(const std::string& n) {
    name = n;
}

void Player::update(float dt) {
    float movement = 0.f;

    if (moveLeft)  movement -= speed;
    if (moveRight) movement += speed;

    // Si los controles están invertidos por un power-up:
    if (reversedControls)
        movement *= -1;

    sprite.move(movement * dt, 0.f);
}

sf::FloatRect Player::getBounds() const {
    return sprite.getGlobalBounds();
}
