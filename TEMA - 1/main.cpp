#include <iostream>
#include <fstream>
#include "Grammar.h"

int main()
{
	std::ifstream input("input.txt");
	Grammar gramatica(input);
	if (gramatica.validateGrammar())
		//afisare() 
		std::cout << "Gramatica corecta";
	else std::cout << "Gramatica incorecta";
}