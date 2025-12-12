#pragma once
#include <SFML/Graphics.hpp>

struct Keeper {
    sf::Sprite sprite;
    float speed = 200.f;
    int direction = 0;

    // Congelamiento por power-up FreezeKeeper
    bool frozen = false;
    float freezeDuration = 1.f;
    sf::Clock freezeClock;

    // Constructor
    Keeper();

    // Actualiza posición y controla congelamiento
    void update(float dt);

    // Devuelve los límites para colisiones
    sf::FloatRect getBounds() const;

    // Congela al portero por un tiempo determinado
    void freeze(float duration = 1.f);
};
