#include "../include/Keeper.h"
#include <random>
#include <ctime>

static std::mt19937 rngKeeper((unsigned)std::time(nullptr));

void Keeper::update(float dt) {
    if (frozen) {
        if (freezeClock.getElapsedTime().asSeconds() > freezeDuration) {
            frozen = false;
        } else {
            return; // congelado, no se mueve
        }
    }

    // Movimiento aleatorio inteligente
    static float moveTimer = 0.f;
    moveTimer += dt;

    if (moveTimer > 0.4f) {
        moveTimer = 0.f;
        std::uniform_int_distribution<int> dist(0, 2); // 0 = izq, 1 = der, 2 = quieto
        int r = dist(rngKeeper);

        if (r == 0)      direction = -1;
        else if (r == 1) direction = 1;
        else             direction = 0;
    }

    sprite.move(direction * speed * dt, 0.f);

    // Mantener dentro de portería
    float x = sprite.getPosition().x;
    if (x < 200) sprite.setPosition(200, sprite.getPosition().y);
    if (x > 600) sprite.setPosition(600, sprite.getPosition().y);
}

sf::FloatRect Keeper::getBounds() const {
    return sprite.getGlobalBounds();
}
