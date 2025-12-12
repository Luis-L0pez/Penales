#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "Player.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>

int main()
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Penalti", sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    std::srand(static_cast<unsigned>(std::time(nullptr)));

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
    // PORTERÍA
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
    goalSprite.setPosition(window.getSize().x * 0.35f, 620);  // un poco más abajo

    // --------------------------
    // PORTERO
    // --------------------------
    sf::Texture keeperTexture;
    if (!keeperTexture.loadFromFile("assets/keeper.png"))
    {
        std::cout << "Error cargando assets/keeper.png\n";
        return 1;
    }
    sf::Sprite keeperSprite;
    keeperSprite.setTexture(keeperTexture);
    keeperSprite.setScale(0.28f, 0.28f);
    keeperSprite.setPosition(window.getSize().x * 0.43f, 670); // un poco más abajo

    // --------------------------
    // JUGADOR
    // --------------------------
    Player player("Luis");
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("assets/player.png"))
    {
        std::cout << "Error cargando assets/player.png\n";
        return 1;
    }
    player.sprite.setTexture(playerTexture);
    player.sprite.setScale(0.85f, 0.85f);
    player.sprite.setPosition(window.getSize().x * 0.40f, window.getSize().y - 350); // un poco más arriba

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
    ballSprite.setPosition(player.sprite.getPosition());

    sf::Vector2f ballVelocity(0.f, 0.f);
    bool ballInMotion = false;
    sf::Vector2f ballTarget(0.f, 0.f);
    float ballSpeed = 10.f;

    int currentPlayer = 1; // Turno jugador 1 o 2

    sf::Vector2f keeperTarget; // Posición a donde se mueve el portero durante el tiro
    bool keeperMoving = false;

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
        // Movimiento jugador con A y D
        // --------------------------
        player.moveLeft  = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
        player.moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
        player.update(dt);

        // --------------------------
        // Detectar dirección de tiro
        // --------------------------
        if (!ballInMotion)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                ballTarget = sf::Vector2f(-1.f, -1.f);
                ballInMotion = true;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                ballTarget = sf::Vector2f(1.f, -1.f);
                ballInMotion = true;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                ballTarget = sf::Vector2f(0.f, -1.f);
                ballInMotion = true;
            }

            if (ballInMotion)
            {
                ballSprite.setPosition(player.sprite.getPosition());

                // Elegir posición aleatoria del portero solo una vez por tiro
                float porteroMinX = goalSprite.getPosition().x;
                float porteroMaxX = goalSprite.getPosition().x + goalSprite.getGlobalBounds().width - keeperSprite.getGlobalBounds().width;
                keeperTarget.x = porteroMinX + static_cast<float>(std::rand() % static_cast<int>(porteroMaxX - porteroMinX));
                keeperTarget.y = keeperSprite.getPosition().y; // mismo Y
                keeperMoving = true;
            }
        }

        // --------------------------
        // Mover balón
        // --------------------------
        if (ballInMotion)
        {
            // Mover balón
            float length = std::sqrt(ballTarget.x * ballTarget.x + ballTarget.y * ballTarget.y);
            sf::Vector2f direction = ballTarget / length;
            ballSprite.move(direction * ballSpeed);

            // Mover portero solo hasta su destino
            if (keeperMoving)
            {
                float dx = keeperTarget.x - keeperSprite.getPosition().x;
                if (std::abs(dx) > 1.f)
                    keeperSprite.move((dx > 0 ? 1.f : -1.f) * 5.f, 0.f);
                else
                    keeperMoving = false;
            }

            // --------------------------
            // Detectar colisión con portero
            // --------------------------
            if (ballSprite.getGlobalBounds().intersects(keeperSprite.getGlobalBounds()))
            {
                std::cout << "Portero detuvo el tiro!\n";
                ballInMotion = false;
                currentPlayer = (currentPlayer == 1) ? 2 : 1;
                ballSprite.setPosition(player.sprite.getPosition());
            }

            // --------------------------
            // Detectar gol
            // --------------------------
            if (ballSprite.getPosition().y < goalSprite.getPosition().y)
            {
                std::cout << "¡Gol del jugador " << currentPlayer << "!\n";
                ballInMotion = false;
                currentPlayer = (currentPlayer == 1) ? 2 : 1;
                ballSprite.setPosition(player.sprite.getPosition());
            }
        }

        // --------------------------
        // Dibujar todo
        // --------------------------
        window.clear();
        window.draw(stadiumSprite);
        window.draw(goalSprite);
        window.draw(keeperSprite);
        window.draw(player.sprite);
        if (ballInMotion)
            window.draw(ballSprite);
        window.display();
    }

    return 0;
}
