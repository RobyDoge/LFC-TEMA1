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
	void SetFinalStates(const std::vector<std::string>& finalStates);		

	bool VerifyAutomaton();
	bool IsDeterministic();
	bool CheckWord(std::vector<std::string>& word);

	void CheckWordRecursive(std::vector<std::string>& word, size_t index, const std::string& currentState, bool& result);
	std::vector<std::string> GenerateWordVector(const std::string& word) const;

	friend std::ostream& operator<<(std::ostream& output, const FiniteAutomaton& finiteAutomaton);


private:
	void SaveToDiskAutomaton() const;
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
