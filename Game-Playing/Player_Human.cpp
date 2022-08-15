#include "Player_Human.h"
#include <iostream>
int AskForChoice(const std::vector<int>& options);

int Player_Human::ChooseAction(const Game& game)
{
	auto player = (GetPlayerId() == 1) ? "player 1" : "player 2";
	std::cout << player << " choose your action\n";
	game.Display();
	std::cout << "turn number: " << std::to_string(game.GetTurnNumber()) + '\n';
    
	auto validActions = game.GetValidActions();
	int chosen = AskForChoice(validActions);
    return chosen;
}



int AskForChoice(const std::vector<int>& options)
{
	auto isAvailable = [options](int chosen) {return std::find(options.begin(), options.end(), chosen) != options.end(); };

	std::string validInput;
	for (int a : options)
	{
		validInput += std::to_string(a) + ", ";
	}

	//ask for an integer
	int chosen = -1;
	std::cout << "choose from: " << validInput + '\n';
	std::cin >> chosen;

	//ask again while not given a valid option
	while (!std::cin.good() || !isAvailable(chosen))
	{
		std::cin.clear();
		std::cin.ignore(100, '\n');
		std::cout << "enter a valid choice: ";
		std::cin >> chosen;
	}

	std::cout << "you chose: " << chosen << "\n";
	return chosen;
}