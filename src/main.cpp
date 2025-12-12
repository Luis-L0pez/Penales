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
    if (!stadiumTexture.loadFromFile("assets/estadio.png")) return 1;
    sf::Sprite stadiumSprite(stadiumTexture);
    stadiumSprite.setScale(
        float(window.getSize().x) / stadiumTexture.getSize().x,
        float(window.getSize().y) / stadiumTexture.getSize().y
    );

    // --------------------------
    // PORTERÍA
    // --------------------------
    sf::Texture goalTexture;
    if (!goalTexture.loadFromFile("assets/arco.png")) return 1;
    sf::Sprite goalSprite(goalTexture);
    goalSprite.setScale(1.5f, 1.5f);
    goalSprite.setPosition(window.getSize().x * 0.35f, 610);

    // --------------------------
    // PORTERO
    // --------------------------
    sf::Texture keeperTexture;
    if (!keeperTexture.loadFromFile("assets/keeper.png")) return 1;
    sf::Sprite keeperSprite(keeperTexture);
    keeperSprite.setScale(0.28f, 0.28f);
    keeperSprite.setPosition(window.getSize().x * 0.43f, 670);

    // --------------------------
    // JUGADOR
    // --------------------------
    Player player("Luis");
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("assets/player.png")) return 1;
    player.sprite.setTexture(playerTexture);
    player.sprite.setScale(0.85f, 0.85f);
    player.sprite.setPosition(window.getSize().x * 0.40f, window.getSize().y - 350);

    // --------------------------
    // BALÓN
    // --------------------------
    sf::Texture ballTexture;
    if (!ballTexture.loadFromFile("assets/ball.png")) return 1;
    sf::Sprite ballSprite(ballTexture);
    ballSprite.setScale(0.05f, 0.05f);
    ballSprite.setPosition(player.sprite.getPosition().x + 50, player.sprite.getPosition().y + 150);

    sf::Vector2f shootDirection(0.f, 0.f);
    bool ballMoving = false;

    // --------------------------
    // MARCADOR
    // --------------------------
    sf::Font font;
    if (!font.loadFromFile("assets/fonts/arial.ttf")) return 1;

    int scorePlayer1 = 0;
    int scorePlayer2 = 0;
    int currentPlayer = 1;

    sf::RectangleShape scoreBackground(sf::Vector2f(350.f, 80.f));
    scoreBackground.setFillColor(sf::Color(0, 0, 0, 180));
    scoreBackground.setOutlineColor(sf::Color::White);
    scoreBackground.setOutlineThickness(3.f);
    scoreBackground.setPosition(window.getSize().x - 370.f, 20.f);

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
    float ballSpeedX = 4.f;
    float ballSpeedY = 6.f;
    float keeperSpeed = 3.f;
    bool keeperMoving = false;
    float keeperTargetX = keeperSprite.getPosition().x;

    // --------------------------
    // LOOP PRINCIPAL
    // --------------------------
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed) window.close();

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
                ballSprite.setPosition(player.sprite.getPosition().x + 50, player.sprite.getPosition().y + 150);
                keeperTargetX = goalSprite.getPosition().x + std::rand() % int(goalSprite.getGlobalBounds().width - keeperSprite.getGlobalBounds().width);
                keeperMoving = true;
            }
        }

        // Mover balón
        if (ballMoving) ballSprite.move(shootDirection);

        // Mover portero
        if (keeperMoving)
        {
            if (std::abs(keeperSprite.getPosition().x - keeperTargetX) > keeperSpeed)
            {
                if (keeperSprite.getPosition().x < keeperTargetX) keeperSprite.move(keeperSpeed, 0.f);
                else keeperSprite.move(-keeperSpeed, 0.f);
            }
            else
            {
                keeperSprite.setPosition(keeperTargetX, keeperSprite.getPosition().y);
                keeperMoving = false;
            }
        }

        // Detección de gol y reset incluso si falla
        if (ballMoving)
        {
            bool scored = false;
            if (ballSprite.getGlobalBounds().intersects(goalSprite.getGlobalBounds()))
            {
                if (ballSprite.getGlobalBounds().intersects(keeperSprite.getGlobalBounds()))
                    std::cout << "¡Portero la tapó!\n";
                else
                {
                    std::cout << "¡Gol del jugador " << currentPlayer << "!\n";
                    if (currentPlayer == 1) scorePlayer1++;
                    else scorePlayer2++;
                    scored = true;
                    scoreText.setString("P1: " + std::to_string(scorePlayer1) + "  |  P2: " + std::to_string(scorePlayer2));
                }
            }

            // Reset balón y portero
            if (scored || ballSprite.getPosition().y < 0 || ballSprite.getPosition().y > window.getSize().y ||
                ballSprite.getPosition().x < 0 || ballSprite.getPosition().x > window.getSize().x)
            {
                ballMoving = false;
                keeperMoving = false;
                player.sprite.setPosition(window.getSize().x * 0.40f, window.getSize().y - 350);
                ballSprite.setPosition(player.sprite.getPosition().x + 50, player.sprite.getPosition().y + 150);
                keeperSprite.setPosition(window.getSize().x * 0.43f, 670);
                currentPlayer = (currentPlayer == 1) ? 2 : 1;
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
