#include "../include/Power.h"
#include "../include/Ball.h"
#include "../include/Keeper.h"
#include "../include/Player.h"
#include <cstdlib>

Power randomPower() {
    int r = std::rand() % 3;
    Power p;
    if (r == 1) p.type = PowerType::SPEED_BOOST;
    else if (r == 2) p.type = PowerType::REVERSE_CONTROLS;
    p.active = (p.type != PowerType::NONE);
    return p;
}

void applyPowerEffectOnShot(Power &power, Ball &ball, Keeper &keeper, Player &player1, Player &player2) {
    if (!power.active) return;

    switch (power.type) {
        case PowerType::SPEED_BOOST:
            ball.activateSpeedBoost(1.5f);
            break;
        case PowerType::REVERSE_CONTROLS:
            player2.reversedControls = true;
            break;
        default: break;
    }

    power.active = false;
    power.type = PowerType::NONE;
}
