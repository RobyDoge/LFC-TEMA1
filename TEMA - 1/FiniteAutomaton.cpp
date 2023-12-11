#include "FiniteAutomaton.h"
#include <algorithm>
#include <ranges>
#include <sstream>
#include <fstream>

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

		if (const auto& outputSymbols = rule.second; 
			!inputSymbols.empty() && !outputSymbols.empty())
		{
			std::string state = inputSymbols[0];
			std::string inputSymbol = outputSymbols[0];
			std::vector<std::string> outputStates(outputSymbols.begin() + 1, outputSymbols.end());

			bool allTerminals = true;
			for (const std::string& s : outputStates)
			{
				if (std::vector<std::string> vt = grammar.GetVt(); 
					std::ranges::find(vt, s) == vt.end())
				{
					allTerminals = false;
					break;
				}
			}

			if (allTerminals)
			{
				outputStates.emplace_back("Final");
			}

			transitions.emplace_back(std::make_pair(state, inputSymbol), outputStates);
		}
	}

	SetStateTransitionFunctions(transitions);
	SetStartingState(grammar.GetS());
	std::vector<std::string> final;
	final.emplace_back("Final");
	SetFinalStates(final);
	SaveToDiskAutomaton();
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

bool FiniteAutomaton::CheckWord(std::vector<std::string>& word)
{
	bool ok = false;
	std::string start = GetStartingState();
	CheckWordRecursive(word, 0, start, ok);

	return ok;
}

std::vector<std::string> FiniteAutomaton::GenerateWordVector(const std::string& word) const
{
	std::vector<std::string> wordVector;

	for (size_t i = 0; i < word.length(); ++i) {
		for (const std::string& alphabetSymbol : m_inputAlphabet) {
			if (word.compare(i, alphabetSymbol.length(), alphabetSymbol) == 0) 
			{
				wordVector.push_back(alphabetSymbol);
				i += alphabetSymbol.length() - 1; 
				break; 
			}
		}
	}
	return wordVector;
}


void FiniteAutomaton::SaveToDiskAutomaton() const
{
	std::ofstream file("Finite Automaton.txt", std::ios::trunc);
	file << *this;
}

void FiniteAutomaton::CheckWordRecursive(std::vector<std::string>& word, const size_t index, const std::string& currentState, bool& result)
{
	if (index == word.size()) 
	{
		result = std::ranges::find(m_finalStates, currentState) != m_finalStates.end();
		return;
	}

	const std::string& inputSymbol = word[index];

	std::vector<std::string> nextStates;

	for ( auto& [inputAndTransitionSymbol, outputSymbol] : m_stateTransitionFunctions) 
	{
		if (auto& [state, transitionSymbol] = inputAndTransitionSymbol; 
			state == currentState && transitionSymbol == inputSymbol) {
			nextStates.insert(nextStates.end(), outputSymbol.begin(), outputSymbol.end());
		}
	}

	for ( auto& nextState : nextStates) 
	{
		CheckWordRecursive(word, index + 1, nextState, result);
		if (result) 
		{
			return;  
		}
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

std::ostream& operator<<(std::ostream& output, const FiniteAutomaton& finiteAutomaton)
{
	output << "Finite Automaton: M = ({";
	for (const auto& state : finiteAutomaton.m_states)
	{
		if(state == finiteAutomaton.m_states[finiteAutomaton.m_states.size()-1])
		{
			output << state;
			break;
		}
		output << state << " , ";
	}
	output << "},{";
	for (const auto& inputSymbol : finiteAutomaton.m_inputAlphabet)
	{
		if(inputSymbol == finiteAutomaton.m_inputAlphabet[finiteAutomaton.m_inputAlphabet.size()-1])
		{
			output << inputSymbol;
			break;
		}
		output << inputSymbol << " , ";
	}
	output << "}, " << finiteAutomaton.m_startingState << ", {";
	for (const auto& finalState : finiteAutomaton.m_finalStates)
	{
		if(finalState == finiteAutomaton.m_finalStates[finiteAutomaton.m_finalStates.size()-1])
		{
			output << finalState;
			break;
		}
		output << finalState << " , ";
	}
	output << "}).\ndelta contains the following:\n";
	
	for (const auto& [transitionFunction, outputStates] : finiteAutomaton.m_stateTransitionFunctions)
	{
		const auto& [state, inputSymbol] = transitionFunction;
		output << " -delta(" << state << ", " << inputSymbol << ") -> {";

		for (const auto& outputState : outputStates)
		{
			if(outputState==outputStates[outputStates.size()-1])
			{
				output << outputState;
				continue;
			}
			output << outputState << ", ";
		}
		output << "}\n";
	}


	output << "q0 = " << finiteAutomaton.m_startingState << "\n";

	output << "F = {";
	for (const auto& finalState : finiteAutomaton.m_finalStates)
	{
		if (finalState == finiteAutomaton.m_finalStates[finiteAutomaton.m_finalStates.size() - 1])
		{
			output << finalState;
			break;
		}
		output << finalState << ", ";
	}
	output << "}\n";

	return output;
}

