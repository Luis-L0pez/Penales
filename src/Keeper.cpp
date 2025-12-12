#include "Keeper.h"  // Cambié a include relativo
#include <iostream>  // Para std::cerr

static sf::Texture keeperTexture;

Keeper::Keeper() {
    speed = 200.f;

    static bool loaded = false;
    if (!loaded) {
        if (!keeperTexture.loadFromFile("assets/keeper.png")) {
            std::cerr << "Error: No se pudo cargar assets/keeper.png. Saliendo...\n";
            exit(-1);  // Salir para evitar crash
        }
        loaded = true;
    }

    sprite.setTexture(keeperTexture);
    sprite.setScale(0.28f, 0.28f);
    sprite.setPosition(400.f, 150.f);  // Esto se sobrescribe en main.cpp, pero está bien
}

void Keeper::update(float dt) {
    float move = speed * dt;
    sprite.move(move, 0.f);

    if (sprite.getPosition().x < 300.f || sprite.getPosition().x > 500.f)
        speed = -speed;
}
