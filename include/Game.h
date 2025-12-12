
#pragma once 
#define GAME_H

#include <vector>
#include <string>

class Player;
class Keeper;
class Ball;
class RoundManager;
class PowerUp;

class Game {
public:
    Game();
    void Start();
    void Update();
    void Render();
    void Run();

private:
    Player* player1;
    Player* player2;
    Keeper* keeper;
    Ball* ball;
    RoundManager* roundManager;

    bool running;
};

#endif
