#include <iostream>
#include <fstream>
#include <set>

#include "FiniteAutomaton.h"

int main()
{
    bool ok = true;
    int number_of_words;
    std::string lastWord;
	std::ifstream input("input.txt");
	Grammar grammar(input);             

	if (grammar.GetGrammarCheck())
    {
	    std::cout << "Grammar is valid!\n";
    }
    else
    {
        ok = false;
        std::cout << "Grammar is not valid! Please check input file!\n";
        return 1;
    }
    if(grammar.IsRegular())
    {
        std::cout << "Grammar is regular!\n";
    }
    else
    {
        ok = false;
        std::cout << "Grammar is not regular! Please check input file!\n";
        return 1;
    }

    if (ok)
    {
	    int choice;
	    FiniteAutomaton automaton(grammar);
        bool automatonGenerated = false;
        std::string word;
        std::vector<std::string> wordToCheck;
        char showSteps;
    	do {
            std::cout << "\nMenu:\n";
            std::cout << "1. Print Grammar\n";
            std::cout << "2. Generate n random words\n";
            std::cout << "3. Generate equivalent Automaton\n";
            std::cout << "4. Check if word is accepted in Automaton\n";
            std::cout << "5. Check if generated word is accepted in Automaton\n";
            std::cout << "0. Exit\n";
            std::cout << "Enter your choice: ";
            std::cin >> choice;

            switch (choice) {
            case 1: 
                std::cout << grammar;
                break;

            case 2:
                std::cout << "How many words do you want to generate?\n";
                std::cin >> number_of_words;
                std::cout << "Should the steps be shown? (Y/N)\n";
                std::cin >> showSteps;
                std::toupper(showSteps);

                while (number_of_words)
                {
                    word = grammar.GetS();
                    if(showSteps=='Y')
                    {
	                    grammar.GenerateRandomWord(word, std::cout, true);
                    }
                    else
                    {
	                    grammar.GenerateRandomWord(word, std::cout, false);
                    }
                	number_of_words--;
                }
                break;

            case 3:
                std::cout << automaton;
                automatonGenerated = true;
                break;

            case 4:
                if(!automatonGenerated)
                {
                    std::cout << "Automatul nu a fost inca generat!\n";
                    break;
                }
                
                std::cout << "What word do you want to check?\n";
                std::cin >> word;
                wordToCheck = automaton.GenerateWordVector(word);
                if (automaton.CheckWord(wordToCheck))
                    std::cout << "Word is accepted!";
                else std::cout << "Word is not accepted!";
                break;

            case 5:
                if (!automatonGenerated)
                {
                    std::cout << "Automatul nu a fost inca generat!\n";
                    break;
                }
                std::cout << "GeneratedWord:\n";
                word = grammar.GetS();
                
            	grammar.GenerateRandomWord(word, std::cout, true);
                lastWord = grammar.GetLastWord();
                wordToCheck = automaton.GenerateWordVector(lastWord);
                std::cout << "\n" << lastWord<< "\n";
                if (automaton.CheckWord(wordToCheck))
                    std::cout << "Word is accepted!";
                else std::cout << "Word is not accepted!";
                break;

            case 0:
                std::cout << "Exiting program.\n";
                break;

            default:
                std::cout << "Invalid choice. Please enter a valid option.\n";
                break;
            }

        } while (choice != 0);
    }
	return 0;
}