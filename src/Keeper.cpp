#include "Keeper.h"
#include <iostream>

Keeper::Keeper() {
    if (!texture.loadFromFile("assets/keeper.png")) {
        std::cerr << "Error: No se pudo cargar assets/keeper.png\n";
        std::exit(EXIT_FAILURE);
    }

    sprite.setTexture(texture);
}