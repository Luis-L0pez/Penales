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

    // ------------------ Estadio ------------------
    sf::Texture stadiumTexture;
    stadiumTexture.loadFromFile("assets/estadio.png");
    sf::Sprite stadiumSprite(stadiumTexture);
    stadiumSprite.setScale(
        float(window.getSize().x)/stadiumTexture.getSize().x,
        float(window.getSize().y)/stadiumTexture.getSize().y
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
    player1.sprite.setScale(0.3f,0.3f);
    player2.sprite.setScale(0.3f,0.3f);
    bool player1Turn = true;

    // ------------------ Balón ------------------
    Ball ball;
    sf::Vector2f shootDirection(0.f,0.f);
    bool ballMoving = false;

    auto resetBall = [&]() {
        ballMoving = false;
        shootDirection = sf::Vector2f(0.f,0.f);
        ball.sprite.setPosition(
            currentPlayer->sprite.getPosition().x + currentPlayer->sprite.getGlobalBounds().width/2 - ball.sprite.getGlobalBounds().width/2,
            currentPlayer->sprite.getPosition().y - ball.sprite.getGlobalBounds().height
        );
    };
    resetBall();

    // ------------------ Portero ------------------
    Keeper keeper;
    keeper.sprite.setScale(0.12f,0.12f);
    keeper.sprite.setPosition(
        goalArea.left + goalArea.width/2 - keeper.sprite.getGlobalBounds().width/2,
        goalArea.top + goalArea.height - keeper.sprite.getGlobalBounds().height + 10.f
    );
    keeper.speed = 120.f;

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
        "Tirar balón: ESPACIO\n"
        "Activar Power-Up: P\n"
        "Gana el primero que haga 5 goles\n\n"
        "Presiona ENTER para comenzar"
    );
    menuText.setPosition(window.getSize().x/2 - menuText.getGlobalBounds().width/2,
                         window.getSize().y/3);

    bool startGame = false;
    while(!startGame && window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type==sf::Event::Closed) window.close();
            else if(event.type==sf::Event::KeyPressed && event.key.code==sf::Keyboard::Enter)
                startGame = true;
        }
        window.clear();
        window.draw(stadiumSprite);
        window.draw(menuText);
        window.display();
    }

    Power currentPower;

    // ------------------ Loop principal ------------------
    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type==sf::Event::Closed) window.close();
        }

        float dt = 1.f/60.f;

        // -------------------- Movimiento jugador actual --------------------
        currentPlayer->moveLeft = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
        currentPlayer->moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
        currentPlayer->update(dt);

        // -------------------- Disparo con ESPACIO --------------------
        if(!ballMoving){
            if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
                // Dirección fija hacia arriba (puedes variar con random si quieres)
                shootDirection = sf::Vector2f(0.f, -500.f);

                ballMoving = true;
                ball.shoot(shootDirection, 500.f);
            }
        }

        // -------------------- Actualizar balón --------------------
        if(ballMoving){
            ball.update(dt);

            // Portero se mueve aleatoriamente solo si la pelota está en movimiento
            float moveDir = ((std::rand()%3)-1) * keeper.speed * dt;
            keeper.sprite.move(moveDir,0.f);

            // Limitar portero al área
            float leftLimit = goalArea.left;
            float rightLimit = goalArea.left + goalArea.width - keeper.sprite.getGlobalBounds().width;
            if(keeper.sprite.getPosition().x < leftLimit)
                keeper.sprite.setPosition(leftLimit, keeper.sprite.getPosition().y);
            if(keeper.sprite.getPosition().x > rightLimit)
                keeper.sprite.setPosition(rightLimit, keeper.sprite.getPosition().y);
        }

        // -------------------- Activar power-up con tecla P --------------------
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::P) && currentPower.active){
            applyPowerEffectOnShot(currentPower, ball, keeper, *currentPlayer, *currentPlayer);
            currentPower.active = false;
        }

        // -------------------- Detectar gol o colisión --------------------
        bool goalScored = goalArea.contains(ball.sprite.getPosition());
        bool hitKeeper = ball.sprite.getGlobalBounds().intersects(keeper.sprite.getGlobalBounds());

        if(goalScored || ball.sprite.getPosition().y < 0 || hitKeeper){
            if(goalScored){
                if(player1Turn) scorePlayer1++;
                else scorePlayer2++;
            }

            // Cambiar turno
            player1Turn = !player1Turn;
            currentPlayer = player1Turn ? &player1 : &player2;

            // Reset del balón al jugador actual
            resetBall();
        }

        // -------------------- Actualizar marcador --------------------
        scoreText.setString(
            "P1: "+std::to_string(scorePlayer1)+"  |  P2: "+std::to_string(scorePlayer2)
        );

        // -------------------- Dibujar --------------------
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