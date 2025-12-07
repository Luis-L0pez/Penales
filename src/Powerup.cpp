#include "../include/PowerUp.h"
#include <random>
#include <ctime>
#include <cmath>

static std::mt19937 pu_rng((unsigned)std::time(nullptr));

Power randomPower() {
    std::uniform_int_distribution<int> dist(1, 4);
    Power p;
    p.type = (PowerType)dist(pu_rng);
    return p;
}

void applyPowerEffectOnShot(const Power &pow, Ball &b, Keeper &k, Player &shooter, Player &opponent) {
    switch (pow.type) {
        case PowerType::SpeedBoost:
            b.velocity *= 1.7f;
            break;

        case PowerType::Curve:
            b.velocity.x += (b.velocity.y < 0 ? 150.f : -150.f);
            break;

        case PowerType::FreezeKeeper:
            k.frozen = true;
            k.freezeClock.restart();
            k.freezeDuration = 1.3f;
            break;

        case PowerType::ReverseControls:
            opponent.reversedControls = true;
            break;

        default:
            break;
    }
}
