#pragma once
#include <string>
#include <vector>


class FiniteAutomaton
{
public:

private:
	std::vector<std::vector<std::string>> m_states;
	std::vector<std::vector<std::string>> m_inputAlphabet;
	std::vector<std::pair<std::string, std::string>> m_stateTransitionFunctions;
	std::string m_startingState;
	std::vector<std::string> m_finalStates;
};
