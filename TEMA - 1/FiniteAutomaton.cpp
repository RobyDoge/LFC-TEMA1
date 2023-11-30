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

bool FiniteAutomaton::CheckWord(const std::string& word)
{
	std::vector<std::string> separatedWord = SeparateWord(word);

	if (separatedWord.empty())
	{
		return false;
	}


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
	output << "}, \u03B4, " << finiteAutomaton.m_startingState << ", {";
	for(const auto& finalState : finiteAutomaton.m_finalStates)
	{
		output << finalState << " , ";
	}
	output << "}).\n \u03B4 contains the following:\n";
	for(const auto& [transitionFunction , outputStates] : finiteAutomaton.m_stateTransitionFunctions)
	{
		const auto& [state, inputSymbol] = transitionFunction;
		output << "\u03B4=(" << state << " , " << inputSymbol << ") = ";
		if(!outputStates.empty())
		{
			output << "{";
			for(const auto& outputState:outputStates)
			{
				output << outputState << " , ";
			}
			output << "}\n";
			continue;
		}
		output << " \u00D8\n";
	}
	return output;
}
