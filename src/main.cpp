#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
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

    // ------------------ Fondo / Estadio ------------------
    sf::Texture stadiumTexture;
    stadiumTexture.loadFromFile("assets/estadio.png");
    sf::Sprite stadiumSprite(stadiumTexture);
    stadiumSprite.setScale(
        float(window.getSize().x) / stadiumTexture.getSize().x,
        float(window.getSize().y) / stadiumTexture.getSize().y
    );

    // ------------------ Portería ------------------
    sf::Texture goalTexture;
    goalTexture.loadFromFile("assets/arco.png");
    sf::Sprite goalSprite(goalTexture);
    goalSprite.setScale(500.f / goalTexture.getSize().x, 180.f / goalTexture.getSize().y);
    goalSprite.setPosition(150.f, 350.f);

    sf::FloatRect goalArea(
        goalSprite.getPosition().x,
        goalSprite.getPosition().y,
        goalSprite.getGlobalBounds().width,
        goalSprite.getGlobalBounds().height
    );

    // ------------------ Jugadores ------------------
    Player player1("Jugador1");
    Player player2("Jugador2");
    Player* currentPlayer = &player1;

    player1.sprite.setPosition(400.f, 500.f);
    player2.sprite.setPosition(400.f, 500.f);

    // Tamaño pequeño que pediste
    player1.sprite.setScale(0.18f, 0.18f);
    player2.sprite.setScale(0.18f, 0.18f);

    bool player1Turn = true;

    // ------------------ Balón ------------------
    Ball ball;
    sf::Vector2f shootDirection(0.f, 0.f);
    bool ballMoving = false;

    auto resetBall = [&]() {
        ballMoving = false;
        shootDirection = sf::Vector2f(0.f, 0.f);

        ball.sprite.setPosition(
            currentPlayer->sprite.getPosition().x +
            currentPlayer->sprite.getGlobalBounds().width / 2 -
            ball.sprite.getGlobalBounds().width / 2,
            currentPlayer->sprite.getPosition().y -
            ball.sprite.getGlobalBounds().height
        );
    };
    resetBall();

    // ------------------ Portero ------------------
    Keeper keeper;
    keeper.sprite.setScale(0.12f, 0.12f);

    keeper.sprite.setPosition(
        goalArea.left + goalArea.width / 2 - keeper.sprite.getGlobalBounds().width / 2,
        goalArea.top + goalArea.height - keeper.sprite.getGlobalBounds().height + 10.f
    );

    keeper.speed = 120.f;

    // Temporizador para movimiento aleatorio
    float keeperTimer = 0;

    // ------------------ Marcador ------------------
    sf::Font font;
    font.loadFromFile("assets/fonts/arial.ttf");

    int scorePlayer1 = 0;
    int scorePlayer2 = 0;

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(30);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setStyle(sf::Text::Bold);
    scoreText.setPosition(20.f, 20.f);

    // ------------------ Menú inicial ------------------
    sf::Text menuText;
    menuText.setFont(font);
    menuText.setCharacterSize(28);
    menuText.setFillColor(sf::Color::Yellow);
    menuText.setStyle(sf::Text::Bold);
    menuText.setString(
        "CONTROLES:\n"
        "Mover jugador: A / D\n"
        "Tirar balon: ESPACIO\n"
        "Power-Up: P\n"
        "Gana el primero en llegar a 5 puntos\n\n"
        "Presiona ENTER para comenzar"
    );
    menuText.setPosition(
        window.getSize().x / 2 - menuText.getGlobalBounds().width / 2,
        window.getSize().y / 3
    );

    bool startGame = false;

    while(!startGame && window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed)
                window.close();
            else if(event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
                startGame = true;
        }

        window.clear();
        window.draw(stadiumSprite);
        window.draw(menuText);
        window.display();
    }

    Power currentPower;

    // ------------------ LOOP PRINCIPAL ------------------
    while(window.isOpen()) {
        sf::Event event;
        while(window.pollEvent(event)) {
            if(event.type == sf::Event::Closed)
                window.close();
        }

        float dt = 1.f / 60.f;

        // ---------------- MOVIMIENTO JUGADOR ----------------
        currentPlayer->moveLeft  = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
        currentPlayer->moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
        currentPlayer->update(dt);

        // ---------------- DISPARO ----------------
        if(!ballMoving && sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            shootDirection = sf::Vector2f(0.f, -500.f);
            ballMoving = true;
            ball.shoot(shootDirection, 500.f);
        }

        // ---------------- ACTUALIZAR BALÓN ----------------
        if(ballMoving) {
            ball.update(dt);

            // Movimiento aleatorio del portero
            keeperTimer += dt;
            if(keeperTimer >= 0.15f) {
                float dir = (std::rand() % 3 - 1) * keeper.speed * dt * 3.f;
                keeper.sprite.move(dir, 0.f);
                keeperTimer = 0;

                // Limites
                float L = goalArea.left;
                float R = goalArea.left + goalArea.width - keeper.sprite.getGlobalBounds().width;

                if(keeper.sprite.getPosition().x < L) keeper.sprite.setPosition(L, keeper.sprite.getPosition().y);
                if(keeper.sprite.getPosition().x > R) keeper.sprite.setPosition(R, keeper.sprite.getPosition().y);
            }
        }

        // ---------------- GOLES / COLISION ----------------
        bool goalScored = goalArea.contains(ball.sprite.getPosition());
        bool hitKeeper  = ball.sprite.getGlobalBounds().intersects(keeper.sprite.getGlobalBounds());
        bool outTop     = ball.sprite.getPosition().y < 0;

        if(goalScored || hitKeeper || outTop) {

            if(goalScored) {
                if(player1Turn) scorePlayer1++;
                else scorePlayer2++;
            }

            // ------------ MUERTE SÚBITA ---------------
            if(scorePlayer1 >= 5 || scorePlayer2 >= 5) {
                if(scorePlayer1 != scorePlayer2) {
                    sf::Text win;
                    win.setFont(font);
                    win.setCharacterSize(40);
                    win.setFillColor(sf::Color::Yellow);

                    if(scorePlayer1 > scorePlayer2)
                        win.setString("¡Jugador 1 GANA!");
                    else
                        win.setString("¡Jugador 2 GANA!");

                    win.setPosition(
                        window.getSize().x / 2 - win.getGlobalBounds().width / 2,
                        window.getSize().y / 2 - win.getGlobalBounds().height / 2
                    );

                    window.clear();
                    window.draw(stadiumSprite);
                    window.draw(win);
                    window.display();

                    sf::sleep(sf::seconds(4));
                    return 0;
                }
            }

            // Cambiar turno
            player1Turn = !player1Turn;
            currentPlayer = player1Turn ? &player1 : &player2;

            resetBall();
        }

        // ---------------- ACTUALIZAR MARCADOR ----------------
        scoreText.setString(
            "P1: " + std::to_string(scorePlayer1) +
            "  |  P2: " + std::to_string(scorePlayer2)
        );

        // ---------------- DIBUJAR ----------------
        window.clear();
        window.draw(stadiumSprite);
        window.draw(goalSprite);
        window.draw(currentPlayer->sprite);
        window.draw(ball.sprite);
        window.draw(keeper.sprite);
        window.draw(scoreText);
        window.display();
    }

    return 0;
}