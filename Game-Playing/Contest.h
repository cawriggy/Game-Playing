#pragma once
#include "Game.h"
#include "Player.h"
#include <map>
class Contest
{
public:
	Game::PlayState PlayGame(Game& game, Player& p1, Player& p2);

	void PlayNGames(Game& game, Player& p1, Player& p2, int n, std::map<Game::PlayState, int>& OutCounts);
};

