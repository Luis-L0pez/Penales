#include "../include/Player.h"

void Player::update(float dt) {
    float dx = 0.f;

    if (!reversedControls) {
        if (moveLeft) dx = -speed;
        if (moveRight) dx =  speed;
    } else { 
        if (moveLeft) dx =  speed;
        if (moveRight) dx = -speed;
    }

    sprite.move(dx * dt, 0);

    // Limitar al campo
    float x = sprite.getPosition().x;
    if (x < 100) sprite.setPosition(100, sprite.getPosition().y);
    if (x > 700) sprite.setPosition(700, sprite.getPosition().y);
}

sf::FloatRect Player::getBounds() const {
    return sprite.getGlobalBounds();
}

std::string Power::name() const {
    switch (type) {
        case PowerType::SpeedBoost: return "SpeedBoost";
        case PowerType::Curve: return "Curve";
        case PowerType::FreezeKeeper: return "FreezeKeeper";
        case PowerType::ReverseControls: return "ReverseControls";
        default: return "None";
    }
}
