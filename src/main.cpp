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

    sf::Texture stadiumTexture;
    if (!stadiumTexture.loadFromFile("assets/estadio.png")) return 1;
    sf::Sprite stadiumSprite(stadiumTexture);
    stadiumSprite.setScale(
        float(window.getSize().x)/stadiumTexture.getSize().x,
        float(window.getSize().y)/stadiumTexture.getSize().y
    );

    Keeper keeper;
    Player player("Luis");
    Ball ball;
    ball.sprite.setPosition(player.sprite.getPosition().x + player.sprite.getGlobalBounds().width/2,
                            player.sprite.getPosition().y - ball.sprite.getGlobalBounds().height);

    sf::Vector2f shootDirection(0.f, 0.f);
    bool ballMoving = false;

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

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float dt = 1.f/60.f;

        player.moveLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
        player.moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
        player.update(dt);

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

        ball.update(dt);
        keeper.update(dt);

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && currentPower.active) {
            applyPowerEffectOnShot(currentPower, ball, keeper, player, player);
            currentPower.active = false;
        }

        if (ball.sprite.getPosition().y < 0) {
            scorePlayer1++;
            ballMoving = false;
            ball.sprite.setPosition(player.sprite.getPosition().x, player.sprite.getPosition().y - 20.f);
        }

        scoreText.setString("P1: " + std::to_string(scorePlayer1) + "  |  P2: " + std::to_string(scorePlayer2));

        window.clear();
        window.draw(stadiumSprite);
        window.draw(player.sprite);
        window.draw(ball.sprite);
        window.draw(keeper.sprite);
        window.draw(scoreText);
        window.display();
    }

    return 0;
}
