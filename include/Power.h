#pragma once
#include <SFML/Graphics.hpp>

// Forward declarations
class Ball;
class Keeper;
class Player;

enum class PowerType {
    NONE,
    SPEED_BOOST,
    REVERSE_CONTROLS
};

struct Power {
    PowerType type = PowerType::NONE;
    bool active = false;
};

// Funciones
Power randomPower();
void applyPowerEffectOnShot(Power &power, Ball &ball, Keeper &keeper, Player &player1, Player &player2);

