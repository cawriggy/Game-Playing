#include "Player_Random.h"
#include <cstdlib>

std::string Player_Random::GetName()
{
    return "random";
}

int Player_Random::ChooseAction(const Game& game)
{
    std::vector<int> validActions;
    game.GetValidActions(validActions);
    return validActions[rand() % validActions.size()];
}
