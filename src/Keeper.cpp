#include "../include/Keeper.h"
#include <cstdio>

Keeper::Keeper() {
    speed = 200.f;
    direction = 1; // Inicialmente se mueve a la derecha
    frozen = false;
    freezeDuration = 1.f;

    static sf::Texture texture;
    if (!texture.loadFromFile("assets/keeper.png")) {
        printf("No se pudo cargar keeper.png\n");
    }

    sprite.setTexture(texture);
    sprite.setScale(0.5f, 0.5f);
    sprite.setPosition(200.f, 100.f);
}

void Keeper::update(float dt) {
    if (frozen) {
        if (freezeClock.getElapsedTime().asSeconds() >= freezeDuration) {
            frozen = false; // Se descongela
        } else {
            return; // No se mueve mientras está congelado
        }
    }

    // Movimiento horizontal automático
    float movement = direction * speed * dt;
    sprite.move(movement, 0.f);

    // Cambia dirección al llegar a los límites
    if (sprite.getPosition().x < 50.f) direction = 1;
    if (sprite.getPosition().x > 350.f) direction = -1;
}

sf::FloatRect Keeper::getBounds() const {
    return sprite.getGlobalBounds();
}

// Método opcional para activar congelamiento desde power-up
void Keeper::freeze(float duration) {
    frozen = true;
    freezeDuration = duration;
    freezeClock.restart();
}
