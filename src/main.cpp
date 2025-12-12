#include <SFML/Graphics.hpp>
#include <iostream>
#include "Player.h"


int main()
{
    sf::RenderWindow window(sf::VideoMode(800, 600), "Penalti");

    // --------------------------
    // CARGAR PORTERÍA
    // --------------------------
    sf::Texture goalTexture;
    if (!goalTexture.loadFromFile("assets/arco.png"))
    {
        std::cout << "Error cargando assets/arco.png\n";
        return 1;
    }
    sf::Sprite goalSprite;
    goalSprite.setTexture(goalTexture);

    // Portería más pequeña, a la derecha y MÁS ABAJO
    goalSprite.setScale(0.35f, 0.35f);
    goalSprite.setPosition(window.getSize().x * 0.60f, 140);  
    // ← aquí la bajé (antes estaba en 40)


    // --------------------------
    // CARGAR PORTERO
    // --------------------------
    sf::Texture keeperTexture;
    if (!keeperTexture.loadFromFile("assets/keeper.png"))
    {
        std::cout << "Error cargando assets/keeper.png\n";
        return 1;
    }
    sf::Sprite keeperSprite;
    keeperSprite.setTexture(keeperTexture);

    // Portero más grande y más abajo
    keeperSprite.setScale(0.10f, 0.10f);
    keeperSprite.setPosition(window.getSize().x * 0.62f, 340);
    // ← bajé un poco más el portero para que siga centrado con la portería


    // --------------------------
    // CARGAR JUGADOR
    // --------------------------
    Player player("Luis");
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("assets/player.png"))
    {
        std::cout << "Error cargando assets/player.png\n";
        return 1;
    }

    player.sprite.setTexture(playerTexture);
    player.sprite.setScale(0.09f, 0.09f);

    // Jugador más adelante
    player.sprite.setPosition(window.getSize().x * 0.50f, window.getSize().y - 260);


    // --------------------------
    // CARGAR BALÓN
    // --------------------------
    sf::Texture ballTexture;
    if (!ballTexture.loadFromFile("assets/ball.png"))
    {
        std::cout << "Error cargando assets/ball.png\n";
        return 1;
    }
    sf::Sprite ballSprite;
    ballSprite.setTexture(ballTexture);

    // Pelota un poquito más grande
    ballSprite.setScale(0.05f, 0.05f);
    ballSprite.setPosition(window.getSize().x * 0.505f, window.getSize().y - 300);


    // --------------------------
    // LOOP PRINCIPAL
    // --------------------------
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color::Green);

        window.draw(goalSprite);
        window.draw(keeperSprite);
        window.draw(player.sprite);
        window.draw(ballSprite);

        window.display();
    }

    return 0;
}
