#include "../include/Background.h"
#include <iostream>

Background::Background() {
    if (!texture.loadFromFile("assets/stadium.png")) {
        std::cout << "ERROR: No se pudo cargar stadium.png\n";
    }
    sprite.setTexture(texture);
}
void Background::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
