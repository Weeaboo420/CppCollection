#include <iostream>

bool IsValidAddress(std::string input)
{
	unsigned int dots = 0;
	for(int i = 0; i < input.size(); i++) { if(input[i] == '.') { dots++; } }
	return dots == 3;
}

int main()
{
	std::cout << "Type an address to ping. Type \"exit\" to stop" << std::endl;
	bool run = true;

	while(run)
	{
		std::cout << "> ";
		std::string input;
		std::getline(std::cin, input);

		if(input.size() > 0)
		{
			if(input == "exit") { run = false; }

			else if(IsValidAddress(input)) 
			{ 
				std::cout << std::endl << "Pinging " << input << ", use ctrl+c to cancel" << std::endl << std::endl; 
				std::string command = "ping " + input + " -c 8";
				system(command.c_str());
				std::cout << std::endl;
			}
		}
	}

	return 0;
}
