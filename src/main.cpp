#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "Player.h"
#include <iostream>

int main()
{
    // --------------------------
    // CREAR VENTANA FULLSCREEN
    // --------------------------
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Penalti", sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    // --------------------------
    // CARGAR FONDO (ESTADIO)
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
    ); // Fondo ocupa toda la ventana

    // --------------------------
    // CARGAR PORTERÍA (ARCO)
    // --------------------------
    sf::Texture goalTexture;
    if (!goalTexture.loadFromFile("assets/arco.png"))
    {
        std::cout << "Error cargando assets/arco.png\n";
        return 1;
    }
    sf::Sprite goalSprite;
    goalSprite.setTexture(goalTexture);
    goalSprite.setScale(
        float(window.getSize().x) / goalTexture.getSize().x * 0.4f,
        float(window.getSize().y) / goalTexture.getSize().y * 0.4f
    );
    goalSprite.setPosition(window.getSize().x / 2.f - goalTexture.getSize().x * 0.2f, 50);

    // --------------------------
    // CARGAR JUGADOR
    // --------------------------
    Player player("Luis");
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("assets/player.png"))
    {
        std::cout << "Error cargando assets/player.png\n";
        return 1;
    }
    player.sprite.setTexture(playerTexture);
    player.sprite.setScale(0.08f, 0.08f); // Jugador mucho más pequeño
    player.sprite.setPosition(window.getSize().x / 2.f - 50, window.getSize().y - 200);

    // --------------------------
    // CARGAR PORTERO
    // --------------------------
    sf::Texture keeperTexture;
    if (!keeperTexture.loadFromFile("assets/keeper.png"))
    {
        std::cout << "Error cargando assets/keeper.png\n";
        return 1;
    }
    sf::Sprite keeperSprite;
    keeperSprite.setTexture(keeperTexture);
    keeperSprite.setScale(0.06f, 0.06f); // Portero mucho más pequeño
    keeperSprite.setPosition(window.getSize().x / 2.f - 45, 120);

    // --------------------------
    // CARGAR BALÓN
    // --------------------------
    sf::Texture ballTexture;
    if (!ballTexture.loadFromFile("assets/ball.png"))
    {
        std::cout << "Error cargando assets/ball.png\n";
        return 1;
    }
    sf::Sprite ballSprite;
    ballSprite.setTexture(ballTexture);
    ballSprite.setScale(0.03f, 0.03f); // Balón más pequeño
    ballSprite.setPosition(window.getSize().x / 2.f - 15, window.getSize().y - 240);

    sf::Vector2f ballVelocity(0.f, 0.f);

    // --------------------------
    // LOOP PRINCIPAL
    // --------------------------
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float dt = 1.f / 60.f;

        // --------------------------
        // INPUT DEL JUGADOR
        // --------------------------
        player.moveLeft  = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
        player.moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
        player.update(dt);

        // --------------------------
        // PATEAR BALÓN
        // --------------------------
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            ballVelocity = sf::Vector2f(0.f, -12.f);
        }
        ballSprite.move(ballVelocity);

        // --------------------------
        // DIBUJAR TODO
        // --------------------------
        window.clear();
        window.draw(stadiumSprite);  // Fondo
        window.draw(goalSprite);     // Portería
        window.draw(player.sprite);  // Jugador
        window.draw(keeperSprite);   // Portero
        window.draw(ballSprite);     // Balón
        window.display();
    }

    return 0;
}
