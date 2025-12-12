#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "Player.h"
#include <iostream>

int main()
{
    // --------------------------
    // CREAR VENTANA
    // --------------------------
    sf::RenderWindow window(sf::VideoMode(1280, 720), "Penalti", sf::Style::Close);
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
    goalSprite.setPosition(300, 50);

    // --------------------------
    // CARGAR JUGADOR
    // --------------------------
    Player player("Luis");

    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("assets/fronts/player.png"))
    {
        std::cout << "Error cargando assets/fronts/player.png\n";
        return 1;
    }
    player.sprite.setTexture(playerTexture);
    player.sprite.setPosition(500, 550);

    // --------------------------
    // CARGAR PORTERO
    // --------------------------
    sf::Texture keeperTexture;
    if (!keeperTexture.loadFromFile("assets/fronts/keeper.png"))
    {
        std::cout << "Error cargando assets/fronts/keeper.png\n";
        return 1;
    }
    sf::Sprite keeperSprite;
    keeperSprite.setTexture(keeperTexture);
    keeperSprite.setPosition(500, 120);

    // --------------------------
    // CARGAR BALÓN
    // --------------------------
    sf::Texture ballTexture;
    if (!ballTexture.loadFromFile("assets/fronts/ball.png"))
    {
        std::cout << "Error cargando assets/fronts/ball.png\n";
        return 1;
    }
    sf::Sprite ballSprite;
    ballSprite.setTexture(ballTexture);
    ballSprite.setPosition(620, 480);

    // Velocidad de la pelota
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
        // DIBUJAR
        // --------------------------
        window.clear();

        window.draw(stadiumSprite);  // Fondo
        window.draw(goalSprite);     // Portería / arco
        window.draw(player.sprite);  // Jugador
        window.draw(keeperSprite);   // Portero
        window.draw(ballSprite);     // Balón

        window.display();
    }

    return 0;
}
