#include <iostream>
#include <fstream>
#include "include/rapidjson/document.h"

using std::cout;
using std::endl;
using std::ifstream;
using std::getline;
using std::string;

using namespace rapidjson;

int main()
{
	ifstream jsonFile ("data.json");
	if(jsonFile.is_open())
	{
		cout << "File loaded" << endl << endl;
		
		string line; //Temporary variable for reading the current line in the file
		string data; //Where the raw json data will be stored

		while(getline(jsonFile, line))
		{
			data += line + "\n"; //Append the new line to data
		}

		Document document;
		document.Parse(data.c_str()); //Parse the data. Rapidjson expects a c-string so we convert it too.

		//A c-string is basically just an array of chars.
		
		cout << document["foods"]["apple"]["average weight"].GetString() << endl;
	
		jsonFile.close();
	}

	return 0;
}
