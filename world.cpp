#include <iostream>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include "FastNoiseLite.h"

using std::cout;
using std::endl;
using std::to_string;
using std::string;
using std::getline;
using std::cin;

#ifdef _WIN32
	#define PLATFORM "WINDOWS"
#else
	#define PLATFORM "UNIX"
#endif

#ifndef WORLD_WIDTH
	#define WORLD_WIDTH 150
#endif

#ifndef WORLD_HEIGHT
	#define WORLD_HEIGHT 30
#endif

void Clear()
{
	if(PLATFORM == "WINDOWS") { system("cls"); }
	else { system("clear"); }
}

void Render(unsigned char data[][WORLD_HEIGHT])
{
	for(int y = WORLD_HEIGHT; y > 0; y--)
	{
		if(y == WORLD_HEIGHT) 
		{ 
			cout << " ";
			for(int i = 0; i < WORLD_WIDTH; i++) { cout << "-"; } 
			cout << endl;
		}

		for(int x = 0; x < WORLD_WIDTH; x++)
		{
			if(x == 0) { cout << "|"; }

			string symbol;
			int currentBlock = data[x][y];

			switch(currentBlock)
			{
				case 0: //Air
					symbol = " ";
					break;
				case 1: //Grass
					symbol = "#";
					break;
				case 2: //Dirt
					symbol = "%";
					break;
				case 3: //Ore
					symbol = "O";
					break;
				default:
					symbol = "?";
					break;
			}

			cout << symbol << "";
		}

		cout << endl;
	}

	cout << " ";
	for(int i = 0; i < WORLD_WIDTH; i++) { cout << "-"; }
	cout << endl;
}

int main()
{
	srand(time(NULL));

	bool run = true;
	while(run)
	{
		Clear();

		unsigned int seed = ((rand() % 100000000) + (rand() % 10000)) * 3;

		unsigned char data[WORLD_WIDTH][WORLD_HEIGHT];

		FastNoiseLite noise(seed);
		noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
		float noiseData[WORLD_WIDTH];

		string seed_string = to_string(seed);

		int offset = 0;
		for(int i = 0; i < seed_string.size(); i++) 
		{ 
			if(i < 2) { offset += seed_string[i]-'0'; }
			else { break; }
		}

		if(offset > WORLD_HEIGHT) { offset = WORLD_HEIGHT; }
		else if(offset < 0) { offset = 0; }

		//Calculate the steepness of the slopes
		float slopeFactor = 0;
		for(int i = 0; i < seed_string.size(); i++) 
		{ 
			slopeFactor += seed_string[i]-'0';
		}

		slopeFactor *= 0.025f;

		//Generate noise data
		int index = 0;
		for(int x = 0; x < WORLD_WIDTH; x++)
		{
			noiseData[index++] = noise.GetNoise((float) x * slopeFactor, (float)offset);
		}
	
		//World population
		for(int y = WORLD_HEIGHT; y > 0; y--)
		{
			for(int x = 0; x < WORLD_WIDTH; x++)
			{
				unsigned char block = 0;
				int xHeight = round(noiseData[x] * WORLD_HEIGHT); //The noise value for the current x value

				if(y == xHeight) { block = 1; } //Grass
				
				else if(y < xHeight) //Dirt and ores
				{ 
					if(rand() % 11 > 8) { block = 3; }
					else { block = 2; }
				}
			
				data[x][y] = block;
			}
		}

		cout << "seed: " << seed << ", slopeFactor: " << slopeFactor << ", offset: " << offset << endl;
		Render(data);

		cout << endl << "Press ENTER to generate new worlds. Type \"exit\" to stop" << endl << "> ";
		string input;
		getline(cin, input);
		
		if(input == "exit") { run = false; }
	}

	return 0;
}
