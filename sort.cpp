#include <iostream>
#include <vector>
#include <chrono>

#define WINDOWS true

using std::cout;
using std::endl;
using std::string;
using std::vector;

int main()
{
	if(WINDOWS) { cout << "C++ Alphabetical Sorter - windows version" << endl << endl; }

	vector<string> unsorted = { "Resident Evil", "Resident Evil 2", "Resident Evil 3", "Leon S. Kennedy", "Ada Wong", "Rebecca Chambers", "Jill Valentine", "Albert Wesker", "Chris Redfield", "Claire Redfield", "William Birkin", "Anette Birkin", "Sherry Birkin" };
	vector<string> sorted;

	cout << "Unsorted vector: " << endl;
	for(int i = 0; i < unsorted.size(); i++) 
	{ 
		cout << unsorted[i];
		if(i != unsorted.size()-1) { cout << ", "; } 
		else { cout << endl << endl; }
	}

	cout << "Sorting..." << endl << endl;
	auto t1 = std::chrono::high_resolution_clock::now();

	unsigned int indexToRemove = 0; //The index of the item inside "unsorted" that we want to remove
	const unsigned int unsortedSize = unsorted.size(); //We copy the starting size here because the size will change
	//and that would mess up the while loop.

	//Sort alphabetically. In c++ we can compare strings as if they were numbers. "Smaller" strings will be listed higher.
	while(sorted.size() != unsortedSize)
	{
		string newElement;

		for(int i = 0; i < unsorted.size(); i++)
		{
			//If newElement is empty then assign the first encountered element to it.
			if(newElement.size() == 0) { newElement = unsorted[i]; indexToRemove = i; }
		
			//If unsorted[i] is "less than" newElement then that should be sorted before newElement.
			else if(unsorted[i] < newElement) { newElement = unsorted[i]; indexToRemove = i; }
		}

		//Add the newly sorted element and also remove it from the original vector so we don't compare it twice.
		sorted.push_back(newElement);
		unsorted.erase(unsorted.begin() + indexToRemove);
	}

	auto t2 = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

	for(int i = 0; i < sorted.size(); i++) { cout << sorted[i] << endl; }
	cout << endl;

	cout << "Execution time: " << (double)duration / 1000 << "ms" << endl;

	if(WINDOWS)
	{
		cout << "Press ENTER to exit... ";
		string temp;
		std::getline(std::cin, temp);
	}

	return 0;
}
