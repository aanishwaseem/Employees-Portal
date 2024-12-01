#pragma once

#include "OfficialTeam.h"
#include "EarnedTeam.h"
#include "CasualTeam.h"
#include "UnpaidTeam.h"
#include "InactivityManager.h"
Unpaid* Unpaid::instance = nullptr;
Official* Official::instance = nullptr;

Earned* Earned::instance = nullptr;
Casual* Casual::instance = nullptr;

InactivityManager* InactivityManager::main = nullptr;
