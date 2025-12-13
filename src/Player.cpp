#include "Player.h"
#include <iostream>

Player::Player(const std::string& name) : name(name) {
    if (!texture.loadFromFile("assets/player.png")) {
        std::cerr << "Error: No se pudo cargar assets/player.png\n";
        std::exit(EXIT_FAILURE);
    }

    sprite.setTexture(texture);
}

void Player::update(float dt) {
    float movement = 0.f;

    if (moveLeft)
        movement -= speed;
    if (moveRight)
        movement += speed;

    sprite.move(movement * dt, 0.f);

    // Limitar dentro de la pantalla (800x600)
    sf::Vector2f pos = sprite.getPosition();
    sf::FloatRect bounds = sprite.getGlobalBounds();

    if (pos.x < 0.f)
        sprite.setPosition(0.f, pos.y);
    if (pos.x > 800.f - bounds.width)
        sprite.setPosition(800.f - bounds.width, pos.y);
}
