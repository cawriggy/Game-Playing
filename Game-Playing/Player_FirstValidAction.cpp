#include "Player_FirstValidAction.h"
#include <assert.h>

std::string Player_FirstValidAction::GetName()
{
    return "first-valid-action";
}

int Player_FirstValidAction::ChooseAction(const Game& game)
{
    std::vector<int> validActions;
    game.GetValidActions(validActions);
    assert(validActions.size() > 0);
    return validActions[0];
}
