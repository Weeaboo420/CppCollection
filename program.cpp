#include <iostream>

using std::cout;
using std::endl;
using std::string;
using std::stof;

bool IsFloat(const string &input)
{
	try { stof(input); return true; }
	catch(...) { return false; }
}

float GetBmi(const float &weight, const float &height)
{
	return (weight / (height * height));
}

int main()
{
	cout << "BMI Calculator" << endl << "Type \"--exit\" to stop" << endl << endl;
	
	bool run = true;
	bool setHeight = true;
	bool setWeight = false;

	string height;
	string weight;

	while(run)
	{
		string input;

		if(setHeight) {	cout << "Height in meters> "; }
		else if(setWeight) { cout << "Weight in kilograms> "; }

		std::getline(std::cin, input);

		if(input.size() > 0)
		{
			if(input == "--exit") { run = false; }
			
			else
			{
				if(setHeight && IsFloat(input)) { height = input; setHeight = false; setWeight = true; }
				
				else if(setWeight && IsFloat(input)) 
				{ 
					weight = input;
					setWeight = false; 
					setHeight = true;
					cout << "Your BMI: " << GetBmi(stof(weight), stof(height)) << endl << endl;
					weight = "";
					height = "";
				}
			}
		}
	}


	return 0;
}
