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

    // ---------- ESTADIO ----------
    sf::Texture stadiumTexture;
    stadiumTexture.loadFromFile("assets/estadio.png");
    sf::Sprite stadium(stadiumTexture);
    stadium.setScale(
        window.getSize().x / (float)stadiumTexture.getSize().x,
        window.getSize().y / (float)stadiumTexture.getSize().y
    );

    // ---------- PORTERÍA ----------
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

    // ---------- BALÓN ----------
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
        goalArea.left + goalArea.width / 2 - keeper.sprite.getGlobalBounds().width / 2,
        goalArea.top + goalArea.height - keeper.sprite.getGlobalBounds().height + 5.f
    );
    keeper.speed = 120.f;
    float keeperTimer = 0.f;

    // ---------- FUENTE ----------
    sf::Font font;
    font.loadFromFile("assets/fonts/arial.ttf");

    // ---------- TEXTO DEL MENÚ ----------
    sf::Text menuText;
    menuText.setFont(font);
    menuText.setCharacterSize(32);
    menuText.setFillColor(sf::Color::White);
    menuText.setString(
        "CLAVALA ⚽\n\n"
        "Juego de Penales\n\n"
        "CONTROLES:\n"
        "Mover jugador: A / D\n"
        "Disparar:\n"
        "  ← Izquierda\n"
        "  ↑ Centro\n"
        "  → Derecha\n\n"
        "Presiona ENTER para comenzar"
    );
    menuText.setPosition(
        window.getSize().x / 2 - menuText.getGlobalBounds().width / 2,
        120.f
    );

    // ---------- MARCADOR ----------
    int score1 = 0, score2 = 0;
    sf::Text scoreText("", font, 28);
    scoreText.setPosition(20, 20);

    // ---------- TEXTO DE VICTORIA ----------
    sf::Text winText("", font, 36);
    winText.setPosition(200, 250);

    GameState state = MENU;
    bool shotPressed = false;

    // ---------- LOOP ----------
    while (window.isOpen()) {
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

            // ---------- DISPARO CON FLECHAS ----------
            if (state == PLAYING &&
                event.type == sf::Event::KeyPressed &&
                !shotPressed &&
                !ballMoving) {

                sf::Vector2f direction(0.f, -1.f);

                if (event.key.code == sf::Keyboard::Left)
                    direction = sf::Vector2f(-0.6f, -1.f);
                else if (event.key.code == sf::Keyboard::Right)
                    direction = sf::Vector2f(0.6f, -1.f);
                else if (event.key.code == sf::Keyboard::Up)
                    direction = sf::Vector2f(0.f, -1.f);
                else
                    continue;

                shotPressed = true;
                ballMoving = true;
                keeperTimer = 0.f;
                ball.shoot(direction, 500.f);
            }

            if (state == WIN &&
                event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape)
                window.close();
        }

        float dt = 1.f / 60.f;

        window.clear();
        window.draw(stadium);

        if (state == MENU) {
            window.draw(menuText);
        }

        if (state == PLAYING) {

            currentPlayer->moveLeft  = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
            currentPlayer->moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
            currentPlayer->update(dt);

            if (ballMoving) {
                ball.update(dt);
                keeperTimer += dt;
                if (keeperTimer > 0.05f) {
                    keeper.sprite.move((std::rand() % 3 - 1) * keeper.speed * dt * 6.f, 0);
                    keeperTimer = 0.f;
                }
            }

            bool goalScored = ball.sprite.getGlobalBounds().intersects(goalArea);
            bool hitKeeper  = ball.sprite.getGlobalBounds().intersects(keeper.sprite.getGlobalBounds());
            bool out        = ball.sprite.getPosition().y < 0;

            if (ballMoving && (goalScored || hitKeeper || out)) {

                if (goalScored && !hitKeeper) {
                    if (player1Turn) score1++;
                    else score2++;
                }

                if ((score1 >= 5 || score2 >= 5) && score1 != score2) {
                    winText.setString(score1 > score2 ?
                        "¡Jugador 1 GANA!\nESC para salir" :
                        "¡Jugador 2 GANA!\nESC para salir");
                    state = WIN;
                } else {
                    player1Turn = !player1Turn;
                    currentPlayer = player1Turn ? &p1 : &p2;
                    resetBall();
                }
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

        if (state == WIN) {
            window.draw(winText);
            window.draw(scoreText);
        }

        window.display();
    }

    return 0;
}