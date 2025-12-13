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
    keeper.speed = 200.f;
    float keeperDir = 1.f;

    // ---------- FUENTE ----------
    sf::Font font;
    font.loadFromFile("assets/fonts/arial.ttf");

    // ---------- MENÚ ----------
    sf::Text menuText(
        "CLAVALA\n\nJuego de Penales\n\n"
        "CONTROLES:\n"
        "Mover jugador: A / D\n"
        "Disparar:\n"
        "Izquierda  Centro  Derecha\n\n"
        "Presiona ENTER para comenzar",
        font, 32
    );
    menuText.setPosition(
        window.getSize().x / 2 - menuText.getGlobalBounds().width / 2,
        120.f
    );

    // ---------- MARCADOR ----------
    int score1 = 0, score2 = 0;
    sf::Text scoreText("", font, 28);
    scoreText.setPosition(20.f, 20.f);

    // ---------- TEXTO GOL ----------
    sf::Text goalText("", font, 42);
    goalText.setFillColor(sf::Color::Yellow);
    goalText.setStyle(sf::Text::Bold);
    bool showGoalText = false;
    float goalTimer = 0.f;

    // ---------- TEXTO VICTORIA ----------
    sf::Text winText("", font, 36);

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
                event.key.code == sf::Keyboard::Enter) {
                state = PLAYING;
            }

            if (state == PLAYING &&
                event.type == sf::Event::KeyPressed &&
                !shotPressed &&
                !ballMoving) {

                sf::Vector2f dir(0.f, -1.f);

                if (event.key.code == sf::Keyboard::Left)  dir = {-0.6f, -1.f};
                if (event.key.code == sf::Keyboard::Right) dir = { 0.6f, -1.f};
                if (event.key.code == sf::Keyboard::Up)    dir = { 0.f, -1.f};

                shotPressed = true;
                ballMoving = true;
                ball.shoot(dir, 600.f);
            }

            if (state == WIN &&
                event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Escape)
                window.close();
        }

        window.clear();
        window.draw(stadium);

        // ---------- MENU ----------
        if (state == MENU) {
            window.draw(menuText);
        }

        // ---------- JUEGO ----------
        if (state == PLAYING) {

            currentPlayer->moveLeft  = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
            currentPlayer->moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
            currentPlayer->update(dt);

            keeper.sprite.move(keeperDir * keeper.speed * dt, 0);

            if (keeper.sprite.getPosition().x < goalArea.left ||
                keeper.sprite.getPosition().x + keeper.sprite.getGlobalBounds().width >
                goalArea.left + goalArea.width) {
                keeperDir *= -1.f;
            }

            if (ballMoving)
                ball.update(dt);

            bool hitKeeper = ball.sprite.getGlobalBounds().intersects(
                keeper.sprite.getGlobalBounds());
            bool goalScored = ball.sprite.getGlobalBounds().intersects(goalArea);
            bool out = ball.sprite.getPosition().y < 0;

            if (ballMoving && (hitKeeper || goalScored || out)) {

                if (goalScored && !hitKeeper) {

                    if (player1Turn) {
                        score1++;
                        goalText.setString("GOOOL JUGADOR 1");
                    } else {
                        score2++;
                        goalText.setString("GOOOL JUGADOR 2");
                    }

                    goalText.setPosition(
                        window.getSize().x / 2 - goalText.getGlobalBounds().width / 2,
                        200.f
                    );

                    showGoalText = true;
                    goalTimer = 1.2f;

                    if (score1 >= 5 || score2 >= 5) {
                        state = WIN;
                        winText.setString(score1 > score2 ?
                            "GANO JUGADOR 1" : "GANO JUGADOR 2");
                        winText.setPosition(
                            window.getSize().x / 2 -
                            winText.getGlobalBounds().width / 2, 250.f);
                    }
                }

                player1Turn = !player1Turn;
                currentPlayer = player1Turn ? &p1 : &p2;
                resetBall();
            }

            scoreText.setString(
                "P1: " + std::to_string(score1) +
                "  |  P2: " + std::to_string(score2));

            window.draw(goal);
            window.draw(p1.sprite);
            window.draw(p2.sprite);
            window.draw(ball.sprite);
            window.draw(keeper.sprite);
            window.draw(scoreText);
        }

        if (showGoalText) {
            goalTimer -= dt;
            if (goalTimer <= 0.f)
                showGoalText = false;
            else
                window.draw(goalText);
        }

        if (state == WIN) {
            window.draw(winText);
            window.draw(scoreText);
        }

        window.display();
    }

    return 0;
}
