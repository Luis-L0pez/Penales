#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "Player.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

int main()
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Penalti", sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    std::srand(std::time(nullptr)); // Para random del portero

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
    goalSprite.setPosition(window.getSize().x * 0.35f, 610);

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
    keeperSprite.setPosition(window.getSize().x * 0.43f, 660);

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
    player.sprite.setPosition(window.getSize().x * 0.40f, window.getSize().y - 250);

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
    ballSprite.setPosition(player.sprite.getPosition().x + 50, player.sprite.getPosition().y + 100);

    sf::Vector2f ballVelocity(0.f, 0.f);

    // --------------------------
    // MARCADOR
    // --------------------------
    sf::Font font;
   if (!font.loadFromFile("assets/Fonts/Arial.ttf")) {
    std::cout << "Error cargando fuente Arial.ttf\n";
    return 1;
}


    int scorePlayer1 = 0;
    int scorePlayer2 = 0;
    int currentPlayer = 1; // 1 o 2

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(50);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(window.getSize().x - 250, 20);
    scoreText.setString("P1: 0  P2: 0");

    bool ballMoving = false;
    sf::Vector2f shootDirection;

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

        // Movimiento del jugador
        player.moveLeft  = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
        player.moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
        player.update(dt);

        // Determinar dirección de tiro con flechas
        if (!ballMoving)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                shootDirection = sf::Vector2f(-7.f, -12.f);
                ballMoving = true;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                shootDirection = sf::Vector2f(7.f, -12.f);
                ballMoving = true;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                shootDirection = sf::Vector2f(0.f, -12.f);
                ballMoving = true;
            }
            if (ballMoving)
            {
                // Posicionar el balón en los pies del jugador
                ballSprite.setPosition(player.sprite.getPosition().x + 50, player.sprite.getPosition().y + 100);
                // Portero aleatorio entre izquierda y derecha del arco
                float keeperX = goalSprite.getPosition().x + std::rand() % int(goalSprite.getGlobalBounds().width - keeperSprite.getGlobalBounds().width);
                keeperSprite.setPosition(keeperX, keeperSprite.getPosition().y);
            }
        }

        // Mover el balón si está en vuelo
        if (ballMoving)
        {
            ballSprite.move(shootDirection);
        }

        // Detección de gol
        if (ballMoving)
        {
            if (ballSprite.getGlobalBounds().intersects(goalSprite.getGlobalBounds()))
            {
                // Verificar si el portero la tapa
                if (ballSprite.getGlobalBounds().intersects(keeperSprite.getGlobalBounds()))
                {
                    std::cout << "¡Portero la tapó!\n";
                }
                else
                {
                    std::cout << "¡Gol del jugador " << currentPlayer << "!\n";
                    if (currentPlayer == 1) scorePlayer1++;
                    else scorePlayer2++;
                    scoreText.setString("P1: " + std::to_string(scorePlayer1) + "  P2: " + std::to_string(scorePlayer2));
                }

                // Reset para el siguiente jugador
                currentPlayer = (currentPlayer == 1) ? 2 : 1;
                ballMoving = false;
                ballSprite.setPosition(player.sprite.getPosition().x + 50, player.sprite.getPosition().y + 100);
                player.sprite.setPosition(window.getSize().x * 0.40f, window.getSize().y - 250);
            }
        }

        // Fin de ronda
        if (scorePlayer1 >= 5 || scorePlayer2 >= 5)
        {
            std::cout << "¡Fin del juego!\n";
            window.close();
        }

        // Dibujar todo
        window.clear();
        window.draw(stadiumSprite);
        window.draw(goalSprite);
        window.draw(keeperSprite);
        window.draw(player.sprite);
        window.draw(ballSprite);
        window.draw(scoreText);
        window.display();
    }

    return 0;
}
