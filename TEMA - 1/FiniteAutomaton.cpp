#include "FiniteAutomaton.h"
#include <algorithm>

bool FiniteAutomaton::VerifyAutomaton()
{
	return StartingStateValidation() &&
		FinalStatesValidation() &&
		StateTransitionFunctionsValidation();
}

bool FiniteAutomaton::StartingStateValidation()
{
	return std::ranges::find(m_states, m_startingState) != m_states.end();
}

bool FiniteAutomaton::FinalStatesValidation()
{
	return std::ranges::all_of(m_finalStates, [&](const auto& state)
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
