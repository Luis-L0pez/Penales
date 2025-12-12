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
    // ESTADIO (NO TOCAR)
    // --------------------------
    sf::Texture stadiumTexture;
    if (!stadiumTexture.loadFromFile("assets/estadio.png"))
    {
        std::cout << "Error cargando assets/estadio.png\n";
        return 1;
    }
    sf::Sprite stadiumSprite;
    stadiumSprite.setTexture(stadiumTexture);
    stadiumSprite.setScale(
        float(window.getSize().x) / stadiumTexture.getSize().x,
        float(window.getSize().y) / stadiumTexture.getSize().y
    );

    // --------------------------
    // PORTERÍA — MÁS GRANDE + MÁS ABAJO
    // --------------------------
    sf::Texture goalTexture;
    if (!goalTexture.loadFromFile("assets/arco.png"))
    {
        std::cout << "Error cargando assets/arco.png\n";
        return 1;
    }
    sf::Sprite goalSprite;
    goalSprite.setTexture(goalTexture);
    goalSprite.setScale(1.5f, 1.5f);                        
    goalSprite.setPosition(window.getSize().x * 0.35f, 700);  // MÁS ABAJO

    // --------------------------
    // PORTERO — MÁS GRANDE + MÁS ABAJO + UN POCO A LA DERECHA
    // --------------------------
    sf::Texture keeperTexture;
    if (!keeperTexture.loadFromFile("assets/keeper.png"))
    {
        std::cout << "Error cargando assets/keeper.png\n";
        return 1;
    }
    sf::Sprite keeperSprite;
    keeperSprite.setTexture(keeperTexture);
    keeperSprite.setScale(0.28f, 0.28f);  // un poquito más grande
    keeperSprite.setPosition(window.getSize().x * 0.43f, 780); // más abajo y un poco a la derecha

    // --------------------------
    // JUGADOR — MÁS GRANDE + MÁS ABAJO + MÁS A LA DERECHA
    // --------------------------
    Player player("Luis");
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("assets/player.png"))
    {
        std::cout << "Error cargando assets/player.png\n";
        return 1;
    }
    player.sprite.setTexture(playerTexture);
    player.sprite.setScale(0.14f, 0.14f);  // más grande
    player.sprite.setPosition(window.getSize().x * 0.50f, window.getSize().y - 250);

    // --------------------------
    // BALÓN
    // --------------------------
    sf::Texture ballTexture;
    if (!ballTexture.loadFromFile("assets/ball.png"))
    {
        std::cout << "Error cargando assets/ball.png\n";
        return 1;
    }
    sf::Sprite ballSprite;
    ballSprite.setTexture(ballTexture);
    ballSprite.setScale(0.05f, 0.05f);
    ballSprite.setPosition(window.getSize().x * 0.55f, window.getSize().y - 250);

    sf::Vector2f ballVelocity(0.f, 0.f);

    // --------------------------
    // LOOP PRINCIPAL
    // --------------------------
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        float dt = 1.f / 60.f;

        // Movimiento del jugador
        player.moveLeft  = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
        player.moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
        player.update(dt);

        // Mover balón al presionar espacio
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
            ballVelocity = sf::Vector2f(0.f, -12.f);

        ballSprite.move(ballVelocity);

        // Dibujar todo
        window.clear();
        window.draw(stadiumSprite);
        window.draw(goalSprite);
        window.draw(keeperSprite);
        window.draw(player.sprite);
        window.draw(ballSprite);
        window.display();
    }

    return 0;
}
