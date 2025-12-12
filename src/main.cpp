#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "Player.h"
#include "Ball.h"
#include "Keeper.h"
#include "Power.h"


int main()
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Penalti", sf::Style::Fullscreen);
    window.setFramerateLimit(60);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

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
    // PORTERO
    // --------------------------
    Keeper keeper;

    // --------------------------
    // JUGADOR
    // --------------------------
    Player player("Luis");

    // --------------------------
    // BALÓN
    // --------------------------
    Ball ball;
    ball.sprite.setPosition(player.sprite.getPosition().x + player.sprite.getGlobalBounds().width/2,
                            player.sprite.getPosition().y - ball.sprite.getGlobalBounds().height);

    sf::Vector2f shootDirection(0.f, 0.f);
    bool ballMoving = false;

    // --------------------------
    // FONT Y MARCADOR
    // --------------------------
    sf::Font font;
    if (!font.loadFromFile("assets/fonts/arial.ttf")) return 1;

    int scorePlayer1 = 0;
    int scorePlayer2 = 0;

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setStyle(sf::Text::Bold);
    scoreText.setPosition(20.f, 20.f);
    scoreText.setString("P1: 0  |  P2: 0");

    // --------------------------
    // MENÚ INICIAL
    // --------------------------
    sf::Text menuText;
    menuText.setFont(font);
    menuText.setCharacterSize(40);
    menuText.setFillColor(sf::Color::Yellow);
    menuText.setStyle(sf::Text::Bold);
    menuText.setString(
        "CONTROLES:\n"
        "Jugador se mueve: A / D\n"
        "Tirar el balon: Flechas Izq, Der, Arriba\n"
        "Activar power-up: Espacio\n"
        "Gana el primero que haga 5 goles\n\n"
        "Presiona ENTER para comenzar"
    );
    menuText.setPosition(window.getSize().x/2 - menuText.getGlobalBounds().width/2,
                         window.getSize().y/3);

    bool startGame = false;
    while (!startGame && window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed) window.close();
            else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
                startGame = true;
        }
        window.clear();
        window.draw(stadiumSprite);
        window.draw(menuText);
        window.display();
    }

    // --------------------------
    // POWER-UP ACTIVO
    // --------------------------
    Power currentPower;

    // --------------------------
    // LOOP PRINCIPAL
    // --------------------------
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed) window.close();

        float dt = 1.f / 60.f;

        // MOVIMIENTO JUGADOR
        player.moveLeft  = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
        player.moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
        player.update(dt);

        // DISPARAR BALÓN
        if (!ballMoving)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                shootDirection = sf::Vector2f(-300.f, -500.f);
                ballMoving = true;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                shootDirection = sf::Vector2f(300.f, -500.f);
                ballMoving = true;
            }
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                shootDirection = sf::Vector2f(0.f, -500.f);
                ballMoving = true;
            }

            if (ballMoving)
            {
                ball.shoot(shootDirection, 500.f);

                // Elegir power-up aleatorio
                int p = std::rand() % 5;
                switch(p)
                {
                    case 1: currentPower.type = PowerType::SPEED_BOOST; break;
                    case 2: currentPower.type = PowerType::CURVE; break;
                    case 3: currentPower.type = PowerType::FREEZE_KEEPER; break;
                    case 4: currentPower.type = PowerType::REVERSE_CONTROLS; break;
                    default: currentPower.type = PowerType::NONE; break;
                }
                currentPower.active = (currentPower.type != PowerType::NONE);
            }
        }

        // ACTUALIZAR BALÓN
        ball.update(dt);

        // ACTUALIZAR PORTERO
        keeper.update(dt);

        // ACTIVAR POWER-UP CON ESPACIO
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && currentPower.active)
        {
            applyPowerEffectOnShot(currentPower, ball, keeper, player, player); // jugador vs sí mismo en este ejemplo
            currentPower.active = false;
        }

        // DETECCIÓN DE GOLES (simple)
        if (ball.sprite.getPosition().y < 0)
        {
            scorePlayer1++;
            ballMoving = false;
            ball.sprite.setPosition(player.sprite.getPosition().x, player.sprite.getPosition().y - 20.f);
        }

        scoreText.setString("P1: " + std::to_string(scorePlayer1) +
                            "  |  P2: " + std::to_string(scorePlayer2));

        // FIN DE JUEGO
        if (scorePlayer1 >= 5 || scorePlayer2 >= 5) window.close();

        // DIBUJAR
        window.clear();
        window.draw(stadiumSprite);
        window.draw(player.sprite);
        window.draw(ball.sprite);
        window.draw(scoreText);
        window.display();
    }

    return 0;
}
