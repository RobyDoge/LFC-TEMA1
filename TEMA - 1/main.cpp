#include <iostream>
#include <fstream>
#include "Grammar.h"

int main()
{
	std::ifstream input("input.txt");
	Grammar gramatica(input);
	std::cout << gramatica;
	std::cout << "Generating a word: \n";
	gramatica.GenerateRandomWord(gramatica.GetS(), std::cout, 1);

	return 0;
}