#include <iostream>
#include <fstream>
#include "Grammar.h"

int main()
{
	std::ifstream input("input.txt");
	Grammar grammar(input);
	std::cout << grammar;
	std::cout << "Generating a word: \n";
	grammar.GenerateRandomWord(grammar.GetS(), std::cout, true);

	if (grammar.IsRegular())
	return 0;
}