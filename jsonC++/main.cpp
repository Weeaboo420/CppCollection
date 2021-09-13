#include "include/nlohmann/json.hpp"
#include "game.hpp"

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <chrono>

using json = nlohmann::json;

bool FileExists(std::string& filename)
{
	std::ifstream file(filename);
	return file.good();
}

int main()
{	
	std::cout << "[Json Serializer]" << std::endl;
	
	bool validFile = false;
	std::string filename;
	std::string rawData;
	
	while(!validFile)
	{
		std::cout << "Json data filename? ";
		std::getline(std::cin, filename);
		
		if(FileExists(filename))
		{
			std::cout  << "File OK" << std::endl;
			validFile = true;			
		}
	}
	
	std::ifstream file(filename);
	json jsonRaw;
	auto t1 = std::chrono::steady_clock::now();
	
	if(file.is_open())
	{
		std::cout << "Reading data... ";
		file >> jsonRaw;
		file.close();
		std::cout << "Done" << std::endl << std::endl;
	}	
	
	std::vector<Game> games;
	
	json gamesJson = jsonRaw["Games"];
	for(int i = 0; i < gamesJson.size(); i++)
	{		
		unsigned int valueIndex = 0;
		json gameData = gamesJson[i];
		
		std::string gameName;
		int gameReleaseYear;
		std::string gameDeveloper;
		
		for(auto& game : gameData.items())
		{
			switch(valueIndex)
			{
				case 0:
					gameDeveloper = game.value();
					break;
				case 1:
					gameName = game.value();
					break;
				case 2:
					gameReleaseYear = game.value();
					break;
			}
			
			valueIndex++;
		}
		
		games.push_back(Game(gameName, gameReleaseYear, gameDeveloper));
	}
	
	std::cout << "Games in database: " << std::endl << "------------------" << std::endl << std::endl;
	for(int i = 0; i < games.size(); i++)
	{
		std::cout << "Title: \"" << games[i].GetName() << "\"" << std::endl;
		std::cout << "Released: " << games[i].GetReleaseYear() << std::endl;
		std::cout << "Developed by: " << games[i].GetDeveloper() << std::endl << std::endl;
	}
	
	auto t2 = std::chrono::steady_clock::now();
	std::cout << "Time elapsed: " << std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count() << "ms" << std::endl;
	
	return 0;
}