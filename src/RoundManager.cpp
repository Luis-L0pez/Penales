#include "../include/RoundManager.h"

RoundManager::RoundManager() {
    reset();
}

void RoundManager::reset() {
    round = 1;
    turn = 1; // jugador 1 inicia
    score1 = 0;
    score2 = 0;
}

bool RoundManager::isGameOver() const {
    return round > maxRounds;
}

void RoundManager::nextTurn() {
    turn = (turn == 1 ? 2 : 1);
    if (turn == 1)
        round++;
}
