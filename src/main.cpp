#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>

#include "Player.h"
#include "Ball.h"
#include "Keeper.h"

enum GameState { MENU, PLAYING, WIN };

int main() {

    sf::RenderWindow window(sf::VideoMode(800, 600), "CLAVALA - Penales");
    window.setFramerateLimit(60);
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    sf::Clock clock;

    // ---------- ESTADIO ----------
    sf::Texture stadiumTexture;
    stadiumTexture.loadFromFile("assets/estadio.png");
    sf::Sprite stadium(stadiumTexture);
    stadium.setScale(
        window.getSize().x / (float)stadiumTexture.getSize().x,
        window.getSize().y / (float)stadiumTexture.getSize().y
    );

    // ---------- PORTERIA ----------
    sf::Texture goalTexture;
    goalTexture.loadFromFile("assets/arco.png");
    sf::Sprite goal(goalTexture);
    goal.setScale(500.f / goalTexture.getSize().x, 180.f / goalTexture.getSize().y);
    goal.setPosition(150.f, 300.f);
    sf::FloatRect goalArea = goal.getGlobalBounds();

    // ---------- JUGADORES ----------
    Player p1("Jugador 1");
    Player p2("Jugador 2");

    p1.sprite.setPosition(400.f, 500.f);
    p2.sprite.setPosition(400.f, 500.f);

    p1.sprite.setScale(0.22f, 0.22f);
    p2.sprite.setScale(0.22f, 0.22f);

    Player* currentPlayer = &p1;
    bool player1Turn = true;

    // ---------- BALON ----------
    Ball ball;
    bool ballMoving = false;

    auto resetBall = [&]() {
        ballMoving = false;
        ball.sprite.setPosition(
            currentPlayer->sprite.getPosition().x +
            currentPlayer->sprite.getGlobalBounds().width / 2 -
            ball.sprite.getGlobalBounds().width / 2,
            currentPlayer->sprite.getPosition().y -
            ball.sprite.getGlobalBounds().height
        );
    };
    resetBall();

    // ---------- PORTERO ----------
    Keeper keeper;
    keeper.sprite.setScale(0.12f, 0.12f);
    keeper.sprite.setPosition(
        goalArea.left + goalArea.width / 2 -
        keeper.sprite.getGlobalBounds().width / 2,
        goalArea.top + goalArea.height -
        keeper.sprite.getGlobalBounds().height + 5.f
    );
    keeper.speed = 260.f;

    bool keeperActive = false;
    float keeperDir = 0.f;

    // ---------- FUENTE ----------
    sf::Font font;
    font.loadFromFile("assets/fonts/arial.ttf");

    // ---------- MARCADOR ----------
    int score1 = 0, score2 = 0;
    sf::Text scoreText("", font, 28);
    scoreText.setPosition(20.f, 20.f);

    // ---------- MENSAJE ----------
    sf::Text msgText("", font, 40);
    msgText.setFillColor(sf::Color::White);
    msgText.setOutlineColor(sf::Color::Black);
    msgText.setOutlineThickness(2.f);

    sf::RectangleShape msgBox;
    msgBox.setFillColor(sf::Color(0, 0, 0, 180));

    bool showMsg = false;
    float msgTimer = 0.f;

    GameState state = MENU;
    bool shotPressed = false;

    // ---------- LOOP ----------
    while (window.isOpen()) {

        float dt = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyReleased)
                shotPressed = false;

            if (state == MENU &&
                event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Enter)
                state = PLAYING;

            // DISPARO SOLO FLECHAS
            if (state == PLAYING &&
                event.type == sf::Event::KeyPressed &&
                !shotPressed &&
                !ballMoving) {

                sf::Vector2f dir;

                if (event.key.code == sf::Keyboard::Left)
                    dir = {-0.6f, -1.f};
                else if (event.key.code == sf::Keyboard::Right)
                    dir = {0.6f, -1.f};
                else if (event.key.code == sf::Keyboard::Up)
                    dir = {0.f, -1.f};
                else
                    continue;

                shotPressed = true;
                ballMoving = true;

                int r = std::rand() % 3;
                keeperDir = (r == 0 ? -1.f : r == 1 ? 0.f : 1.f);
                keeperActive = true;

                ball.shoot(dir, 600.f);
            }
        }

        window.clear();
        window.draw(stadium);

        if (state == PLAYING) {

            currentPlayer->moveLeft  = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
            currentPlayer->moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
            currentPlayer->update(dt);

            if (keeperActive)
                keeper.sprite.move(keeperDir * keeper.speed * dt, 0);

            if (ballMoving)
                ball.update(dt);

            bool hitKeeper = ball.sprite.getGlobalBounds().intersects(
                keeper.sprite.getGlobalBounds());
            bool goalScored = ball.sprite.getGlobalBounds().intersects(goalArea);
            bool out = ball.sprite.getPosition().y < 0;

            if (ballMoving && (hitKeeper || goalScored || out)) {

                keeperActive = false;

                if (goalScored && !hitKeeper) {
                    if (player1Turn) {
                        score1++;
                        msgText.setString("GOOL JUGADOR 1");
                    } else {
                        score2++;
                        msgText.setString("GOOL JUGADOR 2");
                    }
                }
                else if (hitKeeper) {
                    msgText.setString("ATAJADA");
                }
                else {
                    msgText.setString("FALLASTE");
                }

                sf::FloatRect t = msgText.getGlobalBounds();
                msgText.setOrigin(t.width / 2, t.height / 2);
                msgText.setPosition(400.f, 180.f);

                msgBox.setSize({t.width + 40.f, t.height + 30.f});
                msgBox.setOrigin(msgBox.getSize().x / 2, msgBox.getSize().y / 2);
                msgBox.setPosition(400.f, 180.f);

                showMsg = true;
                msgTimer = 1.3f;

                player1Turn = !player1Turn;
                currentPlayer = player1Turn ? &p1 : &p2;
                resetBall();
            }

            scoreText.setString(
                "P1: " + std::to_string(score1) +
                " | P2: " + std::to_string(score2)
            );

            window.draw(goal);
            window.draw(p1.sprite);
            window.draw(p2.sprite);
            window.draw(ball.sprite);
            window.draw(keeper.sprite);
            window.draw(scoreText);
        }

        if (showMsg) {
            msgTimer -= dt;
            if (msgTimer <= 0.f)
                showMsg = false;
            else {
                window.draw(msgBox);
                window.draw(msgText);
            }
        }

        window.display();
    }

    return 0;
}
