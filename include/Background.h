#pragma once
#include <SFML/Graphics.hpp>

class Background {
public:
    Background(const sf::Vector2u& windowSize);
    void draw(sf::RenderWindow& window);

private:
    sf::Texture texture;
    sf::Sprite sprite;
};