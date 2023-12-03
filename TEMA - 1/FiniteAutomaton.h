#pragma once
#include <ostream>
#include <string>
#include <vector>

#include "Grammar.h"


class FiniteAutomaton
{
public:
	FiniteAutomaton() = default;
	FiniteAutomaton(Grammar& grammar);

	std::vector<std::string> GetStates() const;
	void SetStates(const std::vector<std::string>& states);
	std::vector<std::string> GetInputAlphabet() const;
	void SetInputAlphabet(const std::vector<std::string>& inputAlphabet);
	std::vector<std::pair<std::pair<std::string, std::string>, std::vector<std::string>>>
	GetStateTransitionFunctions() const;
	void SetStateTransitionFunctions(
		const std::vector<std::pair<std::pair<std::string, std::string>, std::vector<std::string>>>&
		stateTransitionFunctions);
	std::string GetStartingState() const;
	void SetStartingState(const std::string& startingState);
	std::vector<std::string> GetFinalStates() const;
	void SetFinalStates(const std::vector<std::string>& finalStates);			//getters &setters

	bool VerifyAutomaton();
	bool IsDeterministic();
	bool CheckWord(std::vector<std::string>& word);

	void CheckWordRecursive(std::vector<std::string>& word, size_t index, std::string& currentState, bool& result);
	std::vector<std::string> GenerateWordVector(const std::string& word);


	friend std::ostream& operator<<(std::ostream& output, const FiniteAutomaton& finiteAutomaton);
private:
	
	std::vector<std::string> GetNextStates(std::string currentState, char inputSymbol);
	bool StartingStateValidation();
	bool FinalStatesValidation();
	bool StateTransitionFunctionsValidation();
	std::vector<std::string> SeparateWord(const std::string& word);

private:
	std::vector<std::string> m_states;
	std::vector<std::string> m_inputAlphabet;
	//{ { state , inputSymbol } , outputStates }
	std::vector<std::pair<std::pair<std::string, std::string>, std::vector<std::string>>> m_stateTransitionFunctions;
	std::string m_startingState;
	std::vector<std::string> m_finalStates;
};
