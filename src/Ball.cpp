#include "../include/Ball.h"

Ball::Ball() {
    static sf::Texture texture;
    if (!texture.loadFromFile("assets/ball.png")) {
        // manejo de error
    }
    sprite.setTexture(texture);
    velocity = sf::Vector2f(0.f, 0.f);
}

void Ball::shoot(sf::Vector2f dir, float spd) {
    velocity = dir * spd;
}

void Ball::update(float dt) {
    sprite.move(velocity * dt);
}

sf::FloatRect Ball::getBounds() const {
    return sprite.getGlobalBounds();
}
