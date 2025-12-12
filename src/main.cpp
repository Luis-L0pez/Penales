#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "Player.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <vector>

enum class PowerType { NONE, DOUBLE_POINT, BLOCK_TURN, REMOVE_POINT };

struct Power {
    PowerType type = PowerType::NONE;
    bool active = false;
    int durationFrames = 0;
};

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

    auto resetBallPosition = [&]() {
        ballSprite.setPosition(player.sprite.getPosition().x + player.sprite.getGlobalBounds().width/2 - ballSprite.getGlobalBounds().width/2,
                               player.sprite.getPosition().y + player.sprite.getGlobalBounds().height - ballSprite.getGlobalBounds().height/2);
    };
    resetBallPosition();

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
        "Gana el primero que haga 5 goles\n\n"
        "Presiona ENTER para comenzar"
    );
    menuText.setPosition(window.getSize().x/2 - menuText.getGlobalBounds().width/2, window.getSize().y/3);

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
    // MOVIMIENTO CONTROLADO
    // --------------------------
    float ballSpeedX = 4.f;
    float ballSpeedY = 6.f;
    float keeperSpeed = 3.f;
    bool keeperMoving = false;
    float keeperTargetX = keeperSprite.getPosition().x;

    std::vector<sf::CircleShape> ballTrail;

    Power currentPower;
    bool skipTurnNext = false; // para BLOCK_TURN

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

        // Determinar dirección de tiro
        if (!ballMoving)
        {
            if (skipTurnNext)
            {
                skipTurnNext = false;
                currentPlayer = (currentPlayer == 1) ? 2 : 1;
            }
            else
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
                    resetBallPosition();
                    keeperTargetX = goalSprite.getPosition().x + std::rand() % int(goalSprite.getGlobalBounds().width - keeperSprite.getGlobalBounds().width);
                    keeperMoving = true;
                    ballTrail.clear();

                    // Elegir poder aleatorio
                    int p = std::rand() % 5;
                    switch(p)
                    {
                        case 1: currentPower.type = PowerType::DOUBLE_POINT; currentPower.active = true; break;
                        case 2: currentPower.type = PowerType::BLOCK_TURN; currentPower.active = true; break;
                        case 3: currentPower.type = PowerType::REMOVE_POINT; currentPower.active = true; break;
                        default: currentPower.type = PowerType::NONE; currentPower.active = false; break;
                    }
                }
            }
        }

        // Mover balón
        if (ballMoving)
        {
            ballSprite.move(shootDirection);
            // Estela/trayectoria
            sf::CircleShape dot(5.f);
            dot.setFillColor(sf::Color(255,255,255,150));
            dot.setPosition(ballSprite.getPosition().x + ballSprite.getGlobalBounds().width/2 - dot.getRadius(),
                            ballSprite.getPosition().y + ballSprite.getGlobalBounds().height/2 - dot.getRadius());
            ballTrail.push_back(dot);
        }

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

        // Detección de gol y reset
        if (ballMoving)
        {
            bool scored = false;
            if (ballSprite.getGlobalBounds().intersects(goalSprite.getGlobalBounds()))
            {
                if (ballSprite.getGlobalBounds().intersects(keeperSprite.getGlobalBounds()))
                    std::cout << "¡Portero la tapó!\n";
                else
                {
                    int points = 1;
                    if (currentPower.active && currentPower.type == PowerType::DOUBLE_POINT) points = 2;
                    if (currentPlayer == 1) scorePlayer1 += points;
                    else scorePlayer2 += points;

                    std::cout << "¡Gol del jugador " << currentPlayer << " (" << points << " puntos)!\n";
                    scored = true;
                    scoreText.setString("P1: " + std::to_string(scorePlayer1) + "  |  P2: " + std::to_string(scorePlayer2));

                    // Aplicar poderes de efectos negativos
                    if (currentPower.active && currentPower.type == PowerType::REMOVE_POINT)
                    {
                        if (currentPlayer == 1 && scorePlayer2>0) scorePlayer2--;
                        else if (currentPlayer ==2 && scorePlayer1>0) scorePlayer1--;
                    }
                    if (currentPower.active && currentPower.type == PowerType::BLOCK_TURN)
                        skipTurnNext = true;
                }
            }

            // Reset balón y portero
            if (scored || ballSprite.getPosition().y < 0 || ballSprite.getPosition().y > window.getSize().y ||
                ballSprite.getPosition().x < 0 || ballSprite.getPosition().x > window.getSize().x)
            {
                ballMoving = false;
                keeperMoving = false;
                player.sprite.setPosition(window.getSize().x * 0.40f, window.getSize().y - 350);
                resetBallPosition();
                keeperSprite.setPosition(window.getSize().x * 0.43f, 670);
                currentPlayer = (currentPlayer == 1) ? 2 : 1;
                currentPower.active = false;
                ballTrail.clear();
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
        for(auto &dot: ballTrail) window.draw(dot); // dibujar estela
        window.draw(ballSprite);
        window.draw(scoreBackground);
        window.draw(scoreText);
        window.display();
    }

    return 0;
}
