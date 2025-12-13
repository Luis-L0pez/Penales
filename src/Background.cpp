#include "Background.h"
#include <iostream>

Background::Background(const sf::Vector2u& windowSize) {
    if (!texture.loadFromFile("assets/estadio.png")) {
        std::cerr << "ERROR: No se pudo cargar assets/estadio.png\n";
    }

    sprite.setTexture(texture);
    sprite.setScale(
        windowSize.x / (float)texture.getSize().x,
        windowSize.y / (float)texture.getSize().y
    );
}

void Background::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
