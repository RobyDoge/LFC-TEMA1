#include "FiniteAutomaton.h"
#include <algorithm>
#include <ranges>
#include <sstream>


bool FiniteAutomaton::VerifyAutomaton()
{
	return StartingStateValidation() &&
		FinalStatesValidation() &&
		StateTransitionFunctionsValidation();
}

FiniteAutomaton::FiniteAutomaton(Grammar& grammar)
{
	std::vector<std::string> states;
	states = grammar.GetVn();
	states.emplace_back("Final");
	SetStates(states);
	SetInputAlphabet(grammar.GetVt());

	std::vector<std::pair<std::pair<std::string, std::string>, std::vector<std::string>>> transitions;

	for (const auto& rule : grammar.GetP())
	{
		const auto& inputSymbols = rule.first;
		const auto& outputSymbols = rule.second;

		if (!inputSymbols.empty() && !outputSymbols.empty())
		{
			std::string state = inputSymbols[0];
			std::string inputSymbol = outputSymbols[0];
			std::vector<std::string> outputStates(outputSymbols.begin() + 1, outputSymbols.end());

			bool allTerminals = true;
			for (const std::string& s : outputStates)
			{
				// Check if the symbol is not in the set of terminal symbols
				std::vector<std::string> vt = grammar.GetVt();
				if (std::find(vt.begin(), vt.end(), s) == vt.end())
				{
					allTerminals = false;
					break;
				}
			}

			if (allTerminals)
			{
				outputStates.emplace_back("Final");
			}

			transitions.emplace_back(std::make_pair(std::make_pair(state, inputSymbol), outputStates));
		}
	}

	SetStateTransitionFunctions(transitions);
	SetStartingState(grammar.GetS());
	std::vector<std::string> final;
	final.emplace_back("Final");
	SetFinalStates(final);
}

std::vector<std::string> FiniteAutomaton::GetStates() const
{
	return m_states;
}

void FiniteAutomaton::SetStates(const std::vector<std::string>& states)
{
	m_states = states;
}

std::vector<std::string> FiniteAutomaton::GetInputAlphabet() const
{
	return m_inputAlphabet;
}

void FiniteAutomaton::SetInputAlphabet(const std::vector<std::string>& inputAlphabet)
{
	m_inputAlphabet = inputAlphabet;
}

std::vector<std::pair<std::pair<std::string, std::string>, std::vector<std::string>>> FiniteAutomaton::
GetStateTransitionFunctions() const
{
	return m_stateTransitionFunctions;
}

void FiniteAutomaton::SetStateTransitionFunctions(
	const std::vector<std::pair<std::pair<std::string, std::string>, std::vector<std::string>>>&
	stateTransitionFunctions)
{
	m_stateTransitionFunctions = stateTransitionFunctions;
}

std::string FiniteAutomaton::GetStartingState() const
{
	return m_startingState;
}

void FiniteAutomaton::SetStartingState(const std::string& startingState)
{
	m_startingState = startingState;
}

std::vector<std::string> FiniteAutomaton::GetFinalStates() const
{
	return m_finalStates;
}

void FiniteAutomaton::SetFinalStates(const std::vector<std::string>& finalStates)
{
	m_finalStates = finalStates;
}

bool FiniteAutomaton::IsDeterministic()
{
	return std::ranges::all_of(m_stateTransitionFunctions, [&](const auto& transitionFunction)
	{
		return std::ranges::any_of(transitionFunction.second, [&](const auto& outputState)
		{
			return outputState.size() > 1;
		});
	});
}

bool FiniteAutomaton::CheckWord(std::string& word)
{
	return CheckWordRecursive(word, 0, GetStartingState());
}

std::vector<std::string> FiniteAutomaton::GetNextStates(std::string currentState, char inputSymbol)
{
	std::vector<std::string> nextStates;

	// Iterate over all state transition functions
	for ( auto& transition : m_stateTransitionFunctions) {
		 auto& [transitionState, transitionSymbol] = transition.first;

		// Check if the transition matches the current state and input symbol
		if (transitionState == currentState && transitionSymbol[0] == inputSymbol) {
			nextStates.insert(nextStates.end(), transition.second.begin(), transition.second.end());
		}
	}

	return nextStates;
}

bool FiniteAutomaton::CheckWordRecursive(std::string& word, size_t index, const std::string& currentState)
{
	if (index == word.length()) {
		// Check if the current state is one of the final states
		return currentState == "Final";
	}

	char inputSymbol = word[index];

	// Get next states based on the input symbol
	std::vector<std::string> nextStates = GetNextStates(currentState, inputSymbol);

	// Recursively check the remaining part of the word for each next state
	for ( auto& nextState : nextStates) {
		if (CheckWordRecursive(word, index + 1, nextState)) {
			return true;  // If any path leads to acceptance, return true
		}
	}

	return false;  // All paths from the current state lead to rejection
}

bool FiniteAutomaton::StartingStateValidation()
{
	return std::ranges::find(m_states, m_startingState) != m_states.end();
}

bool FiniteAutomaton::FinalStatesValidation()
{
	return !m_finalStates.empty()&& std::ranges::all_of(m_finalStates, [&](const auto& state)
	{
		return std::ranges::find(m_states, state) != m_states.end();
	});
}

bool FiniteAutomaton::StateTransitionFunctionsValidation()
{
	return std::ranges::all_of(m_stateTransitionFunctions, [&](const auto& transitionFunction)
	{
		return std::ranges::find(m_states, transitionFunction.first.first) != m_states.end() &&
			std::ranges::find(m_inputAlphabet, transitionFunction.first.second) != m_inputAlphabet.end() &&
			std::ranges::all_of(transitionFunction.second, [&](const auto& outputState)
			{
				return std::ranges::find(m_states, outputState) != m_states.end();
			});
	});
}

std::vector<std::string> FiniteAutomaton::SeparateWord(const std::string& word)
{
	std::string newWord = word;
	bool validWord = true;
	for (const auto& symbol : m_inputAlphabet)
	{
		size_t position = newWord.find(symbol);
		while (position != std::string::npos)
		{
			newWord.insert(position + symbol.length(), " ");
			position = newWord.find(symbol, position + symbol.length() + 1);
		}
	}
	std::vector<std::string> stateChangeSequence{};
	std::istringstream nonSplitStream(newWord);
	std::string symbol{};
	while (nonSplitStream >> symbol)
	{
		stateChangeSequence.push_back(symbol);
	}

	if (std::ranges::all_of(stateChangeSequence, [&](const auto& checkSymbol)
			{
				return std::ranges::find(m_inputAlphabet, checkSymbol) != m_inputAlphabet.end();
			}))
	{
		return stateChangeSequence;
	}
	return {};
}

std::ostream& operator<<(std::ostream& output, const FiniteAutomaton& finiteAutomaton)
{
	output << "Finite Automaton: M = ({";
	for (const auto& state : finiteAutomaton.m_states)
	{
		output << state << " , ";
	}
	output << "},{";
	for (const auto& inputSymbol : finiteAutomaton.m_inputAlphabet)
	{
		output << inputSymbol << " , ";
	}
	output << "}, " << finiteAutomaton.m_startingState << ", {";
	for (const auto& finalState : finiteAutomaton.m_finalStates)
	{
		output << finalState << " , ";
	}
	output << "}).\n delta contains the following:\n";

	/*for (const auto& [transitionFunction, outputStates] : finiteAutomaton.m_stateTransitionFunctions)
	{
		const auto& [state, inputSymbol] = transitionFunction;
		output << "delta=(" << state << " , " << inputSymbol << ") = ";

		if (!outputStates.empty())
		{
			output << "{";
			for (const auto& outputState : outputStates)
			{
				output << outputState << " , ";
			}
			output << "}\n";
			continue;
		}
		output << " ∅\n";
	}*/

	
	for (const auto& [transitionFunction, outputStates] : finiteAutomaton.m_stateTransitionFunctions)
	{
		const auto& [state, inputSymbol] = transitionFunction;
		output << "  delta(" << state << ", " << inputSymbol << ") -> {";

		for (const auto& outputState : outputStates)
		{
			output << outputState << ", ";
		}

		output << "}\n";
	}


	output << "q0 = " << finiteAutomaton.m_startingState << "\n";

	output << "F = {";
	for (const auto& finalState : finiteAutomaton.m_finalStates)
	{
		output << finalState << ", ";
	}
	output << "}\n";

	return output;

	return output;
}

