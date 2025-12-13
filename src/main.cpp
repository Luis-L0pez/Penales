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

    // ---------- MENU ----------
    sf::Text menuText(
        "CLAVALA\n\n"
        "PENALES\n\n"
        "CONTROLES:\n"
        "A / D  MOVER\n"
        "FLECHAS  DISPARAR\n\n"
        "ENTER  INICIAR",
        font, 32
    );
    menuText.setFillColor(sf::Color::White);
    menuText.setOutlineColor(sf::Color::Black);
    menuText.setOutlineThickness(2.f);

    sf::FloatRect m = menuText.getGlobalBounds();
    menuText.setOrigin(m.width / 2, m.height / 2);
    menuText.setPosition(400.f, 260.f);

    sf::RectangleShape menuBox;
    menuBox.setSize({m.width + 60.f, m.height + 50.f});
    menuBox.setOrigin(menuBox.getSize().x / 2, menuBox.getSize().y / 2);
    menuBox.setPosition(400.f, 260.f);
    menuBox.setFillColor(sf::Color(0, 0, 0, 190));

    // ---------- MARCADOR ----------
    int score1 = 0, score2 = 0;
    sf::Text scoreText("", font, 28);
    scoreText.setPosition(20.f, 20.f);

    // ---------- TEXTO GOL / ATAJADA ----------
    sf::Text infoText("", font, 42);
    infoText.setFillColor(sf::Color::Yellow);
    infoText.setOutlineColor(sf::Color::Black);
    infoText.setOutlineThickness(2.f);

    bool showInfo = false;
    float infoTimer = 0.f;

    // ---------- TEXTO GANADOR ----------
    sf::Text winText("", font, 36);
    winText.setFillColor(sf::Color::White);

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

            // -------- DISPARO SOLO FLECHAS --------
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

                keeperDir = (std::rand() % 3 - 1);
                keeperActive = true;

                ball.shoot(dir, 600.f);
            }
        }

        window.clear();
        window.draw(stadium);

        // ---------- MENU ----------
        if (state == MENU) {
            window.draw(menuBox);
            window.draw(menuText);
            window.display();
            continue;
        }

        // ---------- JUEGO ----------
        if (state == PLAYING) {

            currentPlayer->moveLeft  = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
            currentPlayer->moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
            currentPlayer->update(dt);

            if (keeperActive) {
                keeper.sprite.move(keeperDir * keeper.speed * dt, 0);

                if (keeper.sprite.getPosition().x < goalArea.left ||
                    keeper.sprite.getPosition().x +
                    keeper.sprite.getGlobalBounds().width >
                    goalArea.left + goalArea.width) {
                    keeperDir *= -1.f;
                }
            }

            if (ballMoving)
                ball.update(dt);

            bool hitKeeper = ball.sprite.getGlobalBounds().intersects(
                keeper.sprite.getGlobalBounds());
            bool goalScored = ball.sprite.getGlobalBounds().intersects(goalArea);
            bool out = ball.sprite.getPosition().y < 0;

            if (ballMoving && (hitKeeper || goalScored || out)) {

                keeperActive = false;
                keeperDir = 0.f;
                ballMoving = false;

                if (goalScored && !hitKeeper) {
                    if (player1Turn) {
                        score1++;
                        infoText.setString("GOOOL JUGADOR 1");
                    } else {
                        score2++;
                        infoText.setString("GOOOL JUGADOR 2");
                    }
                } else {
                    infoText.setString("ATAJADA");
                }

                sf::FloatRect t = infoText.getGlobalBounds();
                infoText.setOrigin(t.width / 2, t.height / 2);
                infoText.setPosition(400.f, 200.f);

                showInfo = true;
                infoTimer = 1.2f;

                if (score1 >= 5 || score2 >= 5) {
                    state = WIN;
                    winText.setString(score1 > score2 ?
                        "GANO JUGADOR 1" : "GANO JUGADOR 2");
                    sf::FloatRect w = winText.getGlobalBounds();
                    winText.setOrigin(w.width / 2, w.height / 2);
                    winText.setPosition(400.f, 260.f);
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

        if (showInfo) {
            infoTimer -= dt;
            if (infoTimer <= 0.f)
                showInfo = false;
            else
                window.draw(infoText);
        }

        if (state == WIN) {
            window.draw(winText);
            window.draw(scoreText);
        }

        window.display();
    }

    return 0;
}
