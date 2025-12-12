#include "Power.h"
#include <cstdlib> // rand()

Power randomPower() {
    Power p;
    int r = rand() % 2;
    p.type = (r == 0) ? PowerType::SPEED_BOOST : PowerType::SLOW_KEEPER;
    p.active = true;
    return p;
}

void applyPowerEffectOnShot(Power &power, Ball &ball, Keeper &keeper, Player &player1, Player &player2) {
    if (!power.active) return;

    switch(power.type) {
        case PowerType::SPEED_BOOST:
            ball.velocity *= 1.5f;
            break;
        case PowerType::SLOW_KEEPER:
            keeper.speed *= 0.5f;
            break;
        default:
            break;
    }

    power.active = false;
}
