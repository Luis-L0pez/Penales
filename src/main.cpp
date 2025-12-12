#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <cstdlib>
#include <ctime>
#include <iostream>  // Para mensajes de error en consola

#include "Player.h"
#include "Ball.h"
#include "Keeper.h"

enum GameState { MENU, PLAYING, WIN };

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Penalti");
    window.setFramerateLimit(60);
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // ------------------ Fondo / Estadio ------------------
    sf::Texture stadiumTexture;
    if (!stadiumTexture.loadFromFile("assets/estadio.png")) {
        std::cerr << "Error: No se pudo cargar assets/estadio.png\n";
        return -1;  // Salir para evitar crash
    }
    sf::Sprite stadiumSprite(stadiumTexture);
    stadiumSprite.setScale(
        float(window.getSize().x) / stadiumTexture.getSize().x,
        float(window.getSize().y) / stadiumTexture.getSize().y
    );

    // ------------------ Portería ------------------
    sf::Texture goalTexture;
    if (!goalTexture.loadFromFile("assets/arco.png")) {
        std::cerr << "Error: No se pudo cargar assets/arco.png\n";
        return -1;
    }
    sf::Sprite goalSprite(goalTexture);
    goalSprite.setScale(500.f / goalTexture.getSize().x, 180.f / goalTexture.getSize().y);
    goalSprite.setPosition(150.f, 350.f);

    sf::FloatRect goalArea = goalSprite.getGlobalBounds();  // Usar getGlobalBounds() directamente

    // ------------------ Jugadores ------------------
    Player player1("Jugador1");
    Player player2("Jugador2");
    Player* currentPlayer = &player1;

    player1.sprite.setPosition(400.f, 500.f);
    player2.sprite.setPosition(400.f, 500.f);

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

    float keeperTimer = 0;

    // ------------------ Marcador ------------------
    sf::Font font;
    if (!font.loadFromFile("assets/fonts/arial.ttf")) {
        std::cerr << "Error: No se pudo cargar assets/fonts/arial.ttf\n";
        return -1;
    }

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
        "Power-Up: P (no implementado)\n"
        "Gana el primero en llegar a 5 puntos\n\n"
        "Presiona ENTER para comenzar"
    );
    menuText.setPosition(
        window.getSize().x / 2 - menuText.getGlobalBounds().width / 2,
        window.getSize().y / 3
    );

    // ------------------ Texto de victoria ------------------
    sf::Text winText;
    winText.setFont(font);
    winText.setCharacterSize(40);
    winText.setFillColor(sf::Color::Yellow);

    GameState state = MENU;

    // ------------------ LOOP PRINCIPAL ------------------
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (state == MENU && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter)
                state = PLAYING;
            if (state == WIN) {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                    // Reiniciar juego
                    scorePlayer1 = 0;
                    scorePlayer2 = 0;
                    player1Turn = true;
                    currentPlayer = &player1;
                    resetBall();
                    state = PLAYING;
                } else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                    window.close();  // Salir del juego
                }
            }
            // Disparo solo en PLAYING, basado en evento (no sostenido)
            if (state == PLAYING && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space && !ballMoving) {
                shootDirection = sf::Vector2f(0.f, -500.f);
                ballMoving = true;
                ball.shoot(shootDirection, 500.f);
            }
        }

        float dt = 1.f / 60.f;

        window.clear();
        window.draw(stadiumSprite);

        if (state == MENU) {
            window.draw(menuText);
        } else if (state == PLAYING) {
            // ---------------- MOVIMIENTO JUGADOR ----------------
            currentPlayer->moveLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
            currentPlayer->moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
            currentPlayer->update(dt);

            // ---------------- ACTUALIZAR BALÓN ----------------
            if (ballMoving) {
                ball.update(dt);

                // Movimiento aleatorio del portero
                keeperTimer += dt;
                if (keeperTimer >= 0.15f) {
                    float dir = (std::rand() % 3 - 1) * keeper.speed * dt * 3.f;
                    keeper.sprite.move(dir, 0.f);
                    keeperTimer = 0;

                    // Límites del keeper
                    float L = goalArea.left;
                    float R = goalArea.left + goalArea.width - keeper.sprite.getGlobalBounds().width;

                    if (keeper.sprite.getPosition().x < L) keeper.sprite.setPosition(L, keeper.sprite.getPosition().y);
                    if (keeper.sprite.getPosition().x > R) keeper.sprite.setPosition(R, keeper.sprite.getPosition().y);
                }
            }

            // ---------------- GOLES / COLISION ----------------
            sf::FloatRect ballBounds = ball.sprite.getGlobalBounds();
            bool goalScored = ballBounds.intersects(goalArea);  // Corrección: usar intersects para detección precisa
            bool hitKeeper = ballBounds.intersects(keeper.sprite.getGlobalBounds());
            bool outTop = ball.sprite.getPosition().y + ballBounds.height < 0;
            bool outLeft = ball.sprite.getPosition().x + ballBounds.width < 0;
            bool outRight = ball.sprite.getPosition().x > window.getSize().x;
            bool outBottom = ball.sprite.getPosition().y > window.getSize().y;

            if (goalScored || hitKeeper || outTop || outLeft || outRight || outBottom) {
                if (goalScored && !hitKeeper) {  // Solo contar gol si intersecta la portería y no choca con keeper
                    if (player1Turn) scorePlayer1++;
                    else scorePlayer2++;
                }

                // Cambiar turno
                player1Turn = !player1Turn;
                currentPlayer = player1Turn ? &player1 : &player2;

                resetBall();

                // Verificar si alguien ganó (primero a 5, desempatar si empate)
                if (scorePlayer1 >= 5 || scorePlayer2 >= 5) {
                    if (scorePlayer1 > scorePlayer2) {
                        winText.setString("¡Jugador 1 GANA!\nPresiona ENTER para reiniciar o ESC para salir");
                        winText.setPosition(
                            window.getSize().x / 2 - winText.getGlobalBounds().width / 2,
                            window.getSize().y / 2 - winText.getGlobalBounds().height / 2
                        );
                        state = WIN;
                    } else if (scorePlayer2 > scorePlayer1) {
                        winText.setString("¡Jugador 2 GANA!\nPresiona ENTER para reiniciar o ESC para salir");
                        winText.setPosition(
                            window.getSize().x / 2 - winText.getGlobalBounds().width / 2,
                            window.getSize().y / 2 - winText.getGlobalBounds().height / 2
                        );
                        state = WIN;
                    }
                    // Si empatan, continúa el juego
                }
            }

            // ---------------- ACTUALIZAR MARCADOR ----------------
            scoreText.setString(
                "P1: " + std::to_string(scorePlayer1) +
                "  |  P2: " + std::to_string(scorePlayer2)
            );

            // ---------------- DIBUJAR ----------------
            window.draw(goalSprite);
            window.draw(currentPlayer->sprite);
            window.draw(ball.sprite);
            window.draw(keeper.sprite);
            window.draw(scoreText);
        } else if (state == WIN) {
            window.draw(winText);
            window.draw(scoreText);
        }

        window.display();
    }

    return 0;
}
