#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include "Player.h"
#include <iostream>

int main()
{
    sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "Penalti", sf::Style::Fullscreen);
    window.setFramerateLimit(60);

    // --------------------------
    // ESTADIO (NO TOCAR)
    // --------------------------
    sf::Texture stadiumTexture;
    if (!stadiumTexture.loadFromFile("assets/estadio.png"))
    {
        std::cout << "Error cargando assets/estadio.png\n";
        return 1;
    }
    sf::Sprite stadiumSprite;
    stadiumSprite.setTexture(stadiumTexture);
    stadiumSprite.setScale(
        float(window.getSize().x) / stadiumTexture.getSize().x,
        float(window.getSize().y) / stadiumTexture.getSize().y
    );

    // --------------------------
    // PORTERÍA — MÁS GRANDE + UN POCO MÁS ARRIBA
    // --------------------------
    sf::Texture goalTexture;
    if (!goalTexture.loadFromFile("assets/arco.png"))
    {
        std::cout << "Error cargando assets/arco.png\n";
        return 1;
    }
    sf::Sprite goalSprite;
    goalSprite.setTexture(goalTexture);
    goalSprite.setScale(1.5f, 1.5f);                        
    goalSprite.setPosition(window.getSize().x * 0.35f, 600);  // subido un poco

    // --------------------------
    // PORTERO — MÁS GRANDE + UN POCO MÁS ARRIBA
    // --------------------------
    sf::Texture keeperTexture;
    if (!keeperTexture.loadFromFile("assets/keeper.png"))
    {
        std::cout << "Error cargando assets/keeper.png\n";
        return 1;
    }
    sf::Sprite keeperSprite;
    keeperSprite.setTexture(keeperTexture);
    keeperSprite.setScale(0.25f, 0.25f);                   
    keeperSprite.setPosition(window.getSize().x * 0.43f, 700); // subido un poco

    // --------------------------
    // JUGADOR — IGUAL QUE ANTES
    // --------------------------
    Player player("Luis");
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("assets/player.jpeg"))
    {
        std::cout << "Error cargando assets/player.jpeg\n";
        return 1;
    }
    player.sprite.setTexture(playerTexture);
   // Escala un poco más grande para que se vea bien
player.sprite.setScale(0.12f, 0.12f);

// Posición un poco más arriba para que quede visible
player.sprite.setPosition(
    window.getSize().x * 0.45f,
    window.getSize().y - 280
);


    // --------------------------
    // LOOP PRINCIPAL
    // --------------------------
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
            if (event.type == sf::Event::Closed)
                window.close();

        float dt = 1.f / 60.f;

        player.moveLeft  = sf::Keyboard::isKeyPressed(sf::Keyboard::A);
        player.moveRight = sf::Keyboard::isKeyPressed(sf::Keyboard::D);
        player.update(dt);

        window.clear();
        window.draw(stadiumSprite);
        window.draw(goalSprite);
        window.draw(keeperSprite);
        window.draw(player.sprite);
        
        window.display();
    }

    return 0;
}
