#pragma once
#include "Ball.h"
#include "Keeper.h"
#include "Player.h"

enum class PowerType { NONE, SPEED_BOOST, SLOW_KEEPER };

struct Power {
    PowerType type = PowerType::NONE;
    bool active = false;
};

// Declaración de funciones
Power randomPower();
void applyPowerEffectOnShot(Power &power, Ball &ball, Keeper &keeper, Player &player1, Player &player2);

