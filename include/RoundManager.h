#pragma once

struct RoundManager {
    int round;
    int turn; // 1 o 2
    int score1;
    int score2;
    const int maxRounds = 5;

    RoundManager();
    void reset();
    void nextTurn();
    bool isGameOver() const;
};
