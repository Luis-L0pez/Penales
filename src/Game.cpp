#include "../include/Game.h"
#include "../include/Player.h"
#include "../include/Ball.h"
#include "../include/Keeper.h"
#include "../include/RoundManager.h"
#include "../include/Power.h"

#include <iostream>

Game::Game() {
    running = true;
    player1 = new Player("Jugador 1");
    player2 = new Player("Jugador 2");
    keeper = new Keeper();
    ball = new Ball();
    roundManager = new RoundManager();
}

void Game::Start() {
    std::cout << "Inicio del juego de penales!\n";
}

void Game::Update() {
    // Aquí va la lógica por ronda
}

void Game::Render() {
    // Aquí va lo visual (texto por ahora)
}

void Game::Run() {
    Start();
    while (running) {
        Update();
        Render();
    }
}
