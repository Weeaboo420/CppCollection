#include "game.hpp"

Game::Game() {}

Game::Game(std::string GameName, int ReleaseYear, std::string Developer)
{
	_gameName = GameName;
	_releaseYear = ReleaseYear;
	_developer = Developer;
}

std::string Game::GetName() { return _gameName; }
int Game::GetReleaseYear() { return _releaseYear; }
std::string Game::GetDeveloper() { return _developer; }