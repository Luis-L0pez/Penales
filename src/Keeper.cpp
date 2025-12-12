#include "../include/Keeper.h"
#include <cstdio>
#include <SFML/Graphics.hpp>

static sf::Texture keeperTexture;

Keeper::Keeper() {
    speed = 200.f;

    static bool loaded = false;
    if (!loaded) {
        if (!keeperTexture.loadFromFile("assets/keeper.png")) {
            printf("No se pudo cargar keeper.png\n");
        }
        loaded = true;
    }

    sprite.setTexture(keeperTexture);
    sprite.setScale(0.28f, 0.28f);
    sprite.setPosition(400.f, 150.f);
}

void Keeper::update(float dt) {
    float move = speed * dt;
    sprite.move(move, 0.f);

    if (sprite.getPosition().x < 300.f || sprite.getPosition().x > 500.f)
        speed = -speed;
}
