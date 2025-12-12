#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include "Player.h"
#include "Ball.h"
#include "Keeper.h"
#include "Power.h"

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Penalti");
    window.setFramerateLimit(60);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // ------------------ Estadio ------------------
    sf::Texture stadiumTexture;
    if (!stadiumTexture.loadFromFile("assets/estadio.png")) return 1;
    sf::Sprite stadiumSprite(stadiumTexture);
    stadiumSprite.setScale(
        float(window.getSize().x)/stadiumTexture.getSize().x,
        float(window.getSize().y)/stadiumTexture.getSize().y
    );

    // ------------------ Portería (arco.png) ------------------
    sf::Texture goalTexture;
    if (!goalTexture.loadFromFile("assets/arco.png")) return 1;
    sf::Sprite goalSprite(goalTexture);
    goalSprite.setScale(
        300.f / goalTexture.getSize().x,  // más ancho
        100.f / goalTexture.getSize().y   // más alto
    );
    goalSprite.setPosition(250.f, 320.f); // más abajo

    sf::FloatRect goalArea(
        goalSprite.getPosition().x,
        goalSprite.getPosition().y,
        goalSprite.getGlobalBounds().width,
        goalSprite.getGlobalBounds().height
    );

    // ------------------ Jugador, Portero y Balón ------------------
    Player player("Luis");
    player.sprite.setPosition(400.f, 500.f);

    Ball ball;
    auto resetBall = [&]() {
        ballMoving = false;
        shootDirection = sf::Vector2f(0.f, 0.f);
        ball.sprite.setPosition(
            player.sprite.getPosition().x + player.sprite.getGlobalBounds().width/2 - ball.sprite.getGlobalBounds().width/2,
            player.sprite.getPosition().y - ball.sprite.getGlobalBounds().height
        );
    };
    bool ballMoving = false;
    sf::Vector2f shootDirection(0.f, 0.f);
    resetBall();

    Keeper keeper;
    keeper.sprite.setScale(0.5f, 0.5f); // portero más pequeño
    keeper.sprite.setPosition(
        goalArea.left + goalArea.width/2 - keeper.sprite.getGlobalBounds().width/2,
        goalArea.top + goalArea.height - keeper.sprite.getGlobalBounds().height
    );
    keeper.speed = 100.f;

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

    Power currentPower;

    // ------------------ Loop Principal ------------------
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float dt = 1.f / 60.f;

        // ------------------ Movimiento jugador ------------------
        player.moveLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
        player.moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
        player.update(dt);

        // ------------------ Disparo del balón ------------------
        if (!ballMoving) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                shootDirection = sf::Vector2f(-300.f, -500.f);
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                shootDirection = sf::Vector2f(300.f, -500.f);
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
                shootDirection = sf::Vector2f(0.f, -500.f);

            if (shootDirection != sf::Vector2f(0.f,0.f)) {
                ballMoving = true;
                ball.shoot(shootDirection, 500.f);
                currentPower = randomPower();
            }
        }

        // ------------------ Actualizar balón ------------------
        ball.update(dt);

        // ------------------ Portero se mueve aleatoriamente ------------------
        if (ballMoving) {
            float moveDir = (std::rand() % 3 - 1) * keeper.speed * dt; // -1,0,1 multiplicado por velocidad
            keeper.sprite.move(moveDir, 0.f);
            float leftLimit = goalArea.left;
            float rightLimit = goalArea.left + goalArea.width - keeper.sprite.getGlobalBounds().width;
            if (keeper.sprite.getPosition().x < leftLimit)
                keeper.sprite.setPosition(leftLimit, keeper.sprite.getPosition().y);
            if (keeper.sprite.getPosition().x > rightLimit)
                keeper.sprite.setPosition(rightLimit, keeper.sprite.getPosition().y);
        }

        // ------------------ Activar power-up ------------------
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && currentPower.active) {
            applyPowerEffectOnShot(currentPower, ball, keeper, player, player);
            currentPower.active = false;
        }

        // ------------------ Detectar gol ------------------
        bool goalScored = goalArea.contains(ball.sprite.getPosition());

        // ------------------ Colisión con portero ------------------
        if (ball.sprite.getGlobalBounds().intersects(keeper.sprite.getGlobalBounds())) {
            resetBall();
        }

        // ------------------ Gol o fuera ------------------
        if (goalScored || ball.sprite.getPosition().y < 0) {
            if (goalScored) scorePlayer1++;
            resetBall();
        }

        // ------------------ Actualizar marcador ------------------
        scoreText.setString(
            "P1: " + std::to_string(scorePlayer1) + "  |  P2: " + std::to_string(scorePlayer2)
        );

        // ------------------ Dibujar ------------------
        window.clear();
        window.draw(stadiumSprite);
        window.draw(goalSprite);
        window.draw(player.sprite);
        window.draw(ball.sprite);
        window.draw(keeper.sprite);
        window.draw(scoreText);
        window.display();
    }

    return 0;
}

