#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "Player.h"
#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Penalti", sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    // --------------------------
    // ESTADIO (NO LO TOCO)
    // --------------------------
    sf::Texture stadiumTexture;
    stadiumTexture.loadFromFile("assets/estadio.png");
    sf::Sprite stadiumSprite(stadiumTexture);
    stadiumSprite.setScale(
        float(window.getSize().x) / stadiumTexture.getSize().x,
        float(window.getSize().y) / stadiumTexture.getSize().y
    );

    // --------------------------
    // PORTERÍA — MÁS GRANDE + MÁS A LA IZQUIERDA
    // --------------------------
    sf::Texture goalTexture;
    goalTexture.loadFromFile("assets/arco.png");

    sf::Sprite goalSprite(goalTexture);
    goalSprite.setScale(0.75f, 0.75f);          // MÁS GRANDE
    goalSprite.setPosition(window.getSize().x * 0.46f, 120); // MÁS A LA IZQUIERDA

    // --------------------------
    // PORTERO — MÁS A LA IZQUIERDA (MISMO TAMAÑO)
    // --------------------------
    sf::Texture keeperTexture;
    keeperTexture.loadFromFile("assets/keeper.png");

    sf::Sprite keeperSprite(keeperTexture);
    keeperSprite.setScale(0.135f, 0.135f);      // MISMO TAMAÑO
    keeperSprite.setPosition(window.getSize().x * 0.50f, 240); // MÁS A LA IZQUIERDA

    // --------------------------
    // JUGADOR — UN PELÍN MÁS ABAJO
    // --------------------------
    Player player("Luis");

    sf::Texture playerTexture;
    playerTexture.loadFromFile("assets/player.png");

    player.sprite.setTexture(playerTexture);
    player.sprite.setScale(0.08f, 0.08f);
    player.sprite.setPosition(window.getSize().x * 0.45f, window.getSize().y - 300); // ABAJO UN POQUITO

    // --------------------------
    // LOOP
    // --------------------------
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        float dt = 1.f / 60.f;

        player.moveLeft  = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
        player.moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
        player.update(dt);

        window.clear();
        window.draw(stadiumSprite);
        window.draw(goalSprite);
        window.draw(keeperSprite);
        window.draw(player.sprite);
        window.display();
    }

    return 0;
}
