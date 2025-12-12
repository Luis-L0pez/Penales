#pragma once
#include <string>

// Forward declarations
struct Ball;
struct Player;
struct Keeper;

// Tipos de Power-ups
enum class PowerType { NONE, SPEED_BOOST, CURVE, FREEZE_KEEPER, REVERSE_CONTROLS };

// Estructura Power
struct Power {
    PowerType type = PowerType::NONE;
    bool active = false;

    // Nombre para debug o menú
    std::string name() const {
        switch (type) {
            case PowerType::NONE: return "None";
            case PowerType::SPEED_BOOST: return "Speed Boost";
            case PowerType::CURVE: return "Curve";
            case PowerType::FREEZE_KEEPER: return "Freeze Keeper";
            case PowerType::REVERSE_CONTROLS: return "Reverse Controls";
        }
        return "Unknown";
    }
};

// Funciones para aplicar o generar power-ups
Power randomPower();
void applyPowerEffectOnShot(const Power &pow, Ball &b, Keeper &k, Player &shooter, Player &opponent);
