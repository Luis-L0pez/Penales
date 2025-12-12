#pragma once
#include <SFML/Graphics.hpp>

class Keeper {
public:
    sf::Sprite sprite;
    sf::Texture texture;   // ✅ LA TEXTURA VIVE AQUÍ
    float speed = 120.f;

    Keeper();
};