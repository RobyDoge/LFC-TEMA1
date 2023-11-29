#pragma once
#include <ostream>
#include <string>
#include <vector>


class FiniteAutomaton
{
public:
	FiniteAutomaton() = default;
	bool VerifyAutomaton();

	friend std::ostream& operator<<(std::ostream& output, const FiniteAutomaton& finiteAutomaton);
private:
	bool StartingStateValidation();
	bool FinalStatesValidation();
	bool StateTransitionFunctionsValidation();

private:
	std::vector<std::string> m_states;
	std::vector<std::string> m_inputAlphabet;
	//{ { state , inputSymbol } , outputStates }
	std::vector<std::pair<std::pair<std::string, std::string>, std::vector<std::string>>> m_stateTransitionFunctions;
	std::string m_startingState;
	std::vector<std::string> m_finalStates;
};
