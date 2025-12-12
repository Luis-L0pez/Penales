#include "../include/Player.h"

Player::Player(const std::string& n) {
    name = n;

    static sf::Texture texture;
    if (!texture.loadFromFile("assets/player.jpeg")) {
        printf("No se pudo cargar player.jpeg\n");
    }
    sprite.setTexture(texture);

    // Ajusta tamaño del sprite si está muy grande
    sprite.setScale(0.5f, 0.5f);

    // Posición inicial
    sprite.setPosition(200, 400);
}

void Player::update(float dt) {
    float movement = 0.f;

    if (moveLeft)  movement -= speed;
    if (moveRight) movement += speed;

    if (reversedControls)
        movement *= -1;

    sprite.move(movement * dt, 0.f);
}

sf::FloatRect Player::getBounds() const {
    return sprite.getGlobalBounds();
}
