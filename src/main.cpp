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
    std::srand(std::time(nullptr));

    // --------------------------
    // ESTADIO
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
    ballSprite.setPosition(player.sprite.getPosition().x + 50, player.sprite.getPosition().y + 120);

    sf::Vector2f shootDirection(0.f, 0.f);
    bool ballMoving = false;

    // --------------------------
    // MARCADOR
    // --------------------------
    sf::Font font;
    if (!font.loadFromFile("assets/fonts/arial.ttf"))
    {
        std::cout << "Error cargando fuente arial.ttf\n";
        return 1;
    }

    int scorePlayer1 = 0;
    int scorePlayer2 = 0;
    int currentPlayer = 1;

    sf::RectangleShape scoreBackground;
    scoreBackground.setSize(sf::Vector2f(320.f, 80.f));
    scoreBackground.setFillColor(sf::Color(0, 0, 0, 180));
    scoreBackground.setOutlineColor(sf::Color::White);
    scoreBackground.setOutlineThickness(3.f);
    scoreBackground.setPosition(window.getSize().x - 350.f, 20.f);

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setStyle(sf::Text::Bold);
    scoreText.setPosition(scoreBackground.getPosition().x + 20.f, scoreBackground.getPosition().y + 20.f);
    scoreText.setString("P1: 0  |  P2: 0");

    // --------------------------
    // MOVIMIENTO CONTROLADO
    // --------------------------
    float ballSpeedX = 5.f;
    float ballSpeedY = 8.f;
    float keeperSpeed = 4.f;
    bool keeperMoving = false;
    float keeperTargetX = keeperSprite.getPosition().x;

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
                shootDirection = sf::Vector2f(-ballSpeedX, -ballSpeedY);
                ballMoving = true;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                shootDirection = sf::Vector2f(ballSpeedX, -ballSpeedY);
                ballMoving = true;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                shootDirection = sf::Vector2f(0.f, -ballSpeedY);
                ballMoving = true;
            }

            if (ballMoving)
            {
                ballSprite.setPosition(player.sprite.getPosition().x + 50, player.sprite.getPosition().y + 120);
                keeperTargetX = goalSprite.getPosition().x + std::rand() % int(goalSprite.getGlobalBounds().width - keeperSprite.getGlobalBounds().width);
                keeperMoving = true;
            }
        }

        // Mover balón
        if (ballMoving)
            ballSprite.move(shootDirection);

        // Mover portero una sola vez
        if (keeperMoving)
        {
            if (keeperSprite.getPosition().x < keeperTargetX)
                keeperSprite.move(keeperSpeed, 0.f);
            else if (keeperSprite.getPosition().x > keeperTargetX)
                keeperSprite.move(-keeperSpeed, 0.f);
        }

        // Detección de gol
        if (ballMoving)
        {
            if (ballSprite.getGlobalBounds().intersects(goalSprite.getGlobalBounds()))
            {
                if (ballSprite.getGlobalBounds().intersects(keeperSprite.getGlobalBounds()))
                    std::cout << "¡Portero la tapó!\n";
                else
                {
                    std::cout << "¡Gol del jugador " << currentPlayer << "!\n";
                    if (currentPlayer == 1) scorePlayer1++;
                    else scorePlayer2++;
                    scoreText.setString("P1: " + std::to_string(scorePlayer1) + "  |  P2: " + std::to_string(scorePlayer2));
                }

                currentPlayer = (currentPlayer == 1) ? 2 : 1;
                ballMoving = false;
                keeperMoving = false;
                ballSprite.setPosition(player.sprite.getPosition().x + 50, player.sprite.getPosition().y + 120);
                player.sprite.setPosition(window.getSize().x * 0.40f, window.getSize().y - 250);
                keeperSprite.setPosition(window.getSize().x * 0.43f, 660);
            }
        }

        // Fin de juego
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
        window.draw(scoreBackground);
        window.draw(scoreText);
        window.display();
    }

    return 0;
}

