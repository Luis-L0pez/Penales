#pragma once
#include "Player.h"
#include "Ball.h"
#include "Keeper.h"

Power randomPower();
void applyPowerEffectOnShot(const Power &pow, Ball &ball, Keeper &keeper, Player &shooter, Player &opponent);
