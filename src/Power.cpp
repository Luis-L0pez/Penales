#include "../include/Power.h"
#include "../include/Ball.h"
#include "../include/Keeper.h"
#include "../include/Player.h"
#include <cstdlib>

std::string Power::name() const {
    switch(type) {
        case PowerType::NONE: return "None";
        case PowerType::SPEED_BOOST: return "Speed Boost";
        case PowerType::CURVE: return "Curve";
        case PowerType::FREEZE_KEEPER: return "Freeze Keeper";
        case PowerType::REVERSE_CONTROLS: return "Reverse Controls";
    }
    return "Unknown";
}

Power randomPower() {
    Power pow;
    int p = std::rand() % 5;
    switch(p) {
        case 1: pow.type = PowerType::SPEED_BOOST; break;
        case 2: pow.type = PowerType::CURVE; break;
        case 3: pow.type = PowerType::FREEZE_KEEPER; break;
        case 4: pow.type = PowerType::REVERSE_CONTROLS; break;
        default: pow.type = PowerType::NONE; break;
    }
    pow.active = (pow.type != PowerType::NONE);
    return pow;
}

void applyPowerEffectOnShot(const Power &pow, Ball &b, Keeper &k, Player &shooter, Player &opponent) {
    switch(pow.type) {
        case PowerType::SPEED_BOOST:
            b.velocity *= 1.7f;
            break;
        case PowerType::CURVE:
            b.velocity.x += (b.velocity.y < 0 ? 150.f : -150.f);
            break;
        case PowerType::FREEZE_KEEPER:
            k.frozen = true;
            k.freezeClock.restart();
            k.freezeDuration = 1.3f;
            break;
        case PowerType::REVERSE_CONTROLS:
            opponent.reversedControls = true;
            break;
        default: break;
    }
}
