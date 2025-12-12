#pragma once
#include <string>

struct Ball;
struct Player;
struct Keeper;

enum class PowerType { NONE, SPEED_BOOST, CURVE, FREEZE_KEEPER, REVERSE_CONTROLS };

struct Power {
    PowerType type = PowerType::NONE;
    bool active = false;
    std::string name() const;
};

Power randomPower();
void applyPowerEffectOnShot(const Power &pow, Ball &b, Keeper &k, Player &shooter, Player &opponent);
