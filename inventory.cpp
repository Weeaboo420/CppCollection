#include <iostream>
#include <vector>

#ifdef __WIN32
	#define PLATFORM "WINDOWS"
#else
	#define PLATFORM "UNIX"
#endif

void Clear()
{
	if(PLATFORM == "WINDOWS") { system("cls"); }
	else { system("clear"); }
}

class Item
{
	private:
		std::string _name = "NO ITEM";
		std::string _description = "NO DESCRIPTION";
	public:
		Item() {};
		Item(std::string name) { _name = name; }
		Item(std::string name, std::string description) { _name = name; _description = description; }
		
		std::string name() const { return _name; }
		std::string description() const { return _description; }
};

int main()
{
	const std::vector<Item> items { 
		Item("Apple", "A medium size fruit that is usually sweet."), 
		Item("Car", "A vehicle with four or more wheels that moves with the help of an engine."), 
		Item("C++", "An old programming language from 1985 that is still used and in development to this day."),
		Item("Minecraft", "A computer game developed by Mojang that released in 2011."),
		Item("Computer", "An electrical device used in home and workspace environments that is usually connected to a monitor, keyboard, mouse and other peripherals.")
	};

	const int maxViewableItems = 4; //How many items can be seen while scrolling

	bool run = true;
	bool showDesc = false;
	int scrollIndex = 0;

	while(run)
	{
		Clear();
		std::cout << "Usage: w, s to scroll. Type \"desc\" for item description. Type \"exit\" to stop" << std::endl << std::endl;
		std::cout << "Items:" << std::endl;

		int render_start = scrollIndex-1;
		unsigned int rendered = 0;

		if(scrollIndex >= items.size() - maxViewableItems) { render_start = items.size() - maxViewableItems; }

		for(int i = render_start; i < scrollIndex + maxViewableItems; i++)
		{
			if(i < 0) { i = 0; }

			if(i <= items.size() - 1 && i >= 0 && rendered < maxViewableItems)
			{
				bool selected = scrollIndex == i;
				std::string prefix = "   ";
				if(selected) { prefix = " > "; }

				std::string description = "";
				if(showDesc && selected) { description = ": " + items[scrollIndex].description(); }

				std::cout << prefix << items[i].name() << description << std::endl;
				rendered++;
			}
		}

		std::cout << "> ";
		std::string input;
		std::getline(std::cin, input);

		if(input == "exit") { run = false; }
		else if(input == "w") { scrollIndex--; if(showDesc) { showDesc = false; }  }
		else if(input == "s") { scrollIndex++; if(showDesc) { showDesc = false; } }

		else if(input == "desc") { showDesc = !showDesc; }

		if(scrollIndex < 0) { scrollIndex = items.size()-1; }
		else if(scrollIndex > items.size()-1) { scrollIndex = 0; }
	}

	return 0;
}
