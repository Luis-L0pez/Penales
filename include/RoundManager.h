#pragma once
struct RoundManager {
    int turn = 1;
    int score1 = 0;
    int score2 = 0;

    void nextTurn() {
        turn = (turn == 1) ? 2 : 1;
    }

    bool hasWinner() const {
        if (score1 >= 5 || score2 >= 5)
            return score1 != score2;
        return false;
    }
};
