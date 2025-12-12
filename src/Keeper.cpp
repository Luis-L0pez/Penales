#include "../include/Keeper.h"

Keeper::Keeper() {
    static sf::Texture texture;
    if (!texture.loadFromFile("front/keeper.png")) {
        printf("No se pudo cargar keeper.png\n");
    }

    sprite.setTexture(texture);
    sprite.setScale(0.5f, 0.5f);
    sprite.setPosition(200, 100);
}

void Keeper::update(float dt) {
    float movement = direction * speed * dt;
    sprite.move(movement, 0);

    if (sprite.getPosition().x < 50)
        direction = 1;
    if (sprite.getPosition().x > 350)
        direction = -1;
}

