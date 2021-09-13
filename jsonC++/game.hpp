#ifndef GAME_HPP
#define GAME_HPP

#include <string>
#include "include/nlohmann/json.hpp"

using json = nlohmann::json;

class Game
{
	private:
		std::string _gameName;
		int _releaseYear;
		std::string _developer;
	public:
		Game();
		Game(std::string GameName, int ReleaseYear, std::string Developer);
		
		std::string GetName();
		int GetReleaseYear();
		std::string GetDeveloper();
};

#endif