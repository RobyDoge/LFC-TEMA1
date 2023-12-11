#include "Grammar.h"
#include <fstream>
#include <numeric>
#include <random>
#include <ranges>

Grammar::Grammar(std::ifstream& input)
{
	std::string aux;
	std::getline(input, aux);				
	std::istringstream nonSplitStream(aux);		
	m_vn = SeparateBySpaces(nonSplitStream);	
	std::getline(input, aux);

	nonSplitStream.clear();
	nonSplitStream.seekg(0);

	nonSplitStream.str(aux);
	m_vt = SeparateBySpaces(nonSplitStream);

	std::getline(input, m_s);

	m_p = CreateVectorP(input);
	input.close();

	if (!VerifyGrammar())
	{
		m_p.clear();
		m_s.erase();
		m_vn.clear();
		m_vt.clear();
		throw std::exception("Incorrect Grammar");
	}
}

std::vector<std::string> Grammar::GetVn()
{
	return m_vn;
}

std::vector<std::string> Grammar::GetVt()
{
	return m_vt;
}

std::string Grammar::GetS()
{
	return m_s;
}

std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> Grammar::GetP()
{
	return m_p;
}

bool Grammar::GetGrammarCheck()
{
	return this->VerifyGrammar();
}

bool Grammar::IsRegular()
{
	return std::ranges::all_of(m_p, [&](const auto& rule)
	{
		const auto& [input, output] = rule;
		return output.size() <= 2 &&
			input.size() <= 1 &&
			std::ranges::find(m_vt, input[0]) == m_vt.end() &&
			std::ranges::find(m_vn, output[0]) == m_vn.end() &&
			!(output.size() == 2 && std::ranges::find(m_vt, output[1]) != m_vt.end());
	});

}


std::vector<std::string> Grammar::SeparateBySpaces(std::istringstream& nonSplitStream)
{
	std::vector<std::string> auxVector;
	std::string word;
	while (nonSplitStream >> word)
	{
		auxVector.push_back(word);
	}
	return auxVector;
}

std::vector<std::string> Grammar::SeparateByComma(std::istringstream& nonSplitStream)
{
	std::vector<std::string> auxVector;
	std::string word;
	while (std::getline(nonSplitStream, word, ','))
	{
		auxVector.push_back(word);
	}
	return auxVector;
}

std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> Grammar::CreateVectorP(std::ifstream& input)
{
	std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> auxP;
	while (!input.eof())
	{
		std::string nonSplitHalfOfRule;
		input >> nonSplitHalfOfRule;
		std::istringstream nonSplitHalfOfRuleStream(nonSplitHalfOfRule);

		std::vector<std::string> aux1 = SeparateByComma(nonSplitHalfOfRuleStream);

		input >> nonSplitHalfOfRule;


		nonSplitHalfOfRuleStream.clear();
		nonSplitHalfOfRuleStream.str(nonSplitHalfOfRule);

		std::vector<std::string> aux2 = SeparateByComma(nonSplitHalfOfRuleStream);

		auxP.emplace_back(aux1, aux2);
	}

	return auxP;
}

bool Grammar::VerifyGrammar()
{
	for (const std::string& vn : m_vn)
	{
		if (std::ranges::find(m_vt, vn) != m_vt.end())
		{
			return false;
		}
	}

	if (std::ranges::find(m_vn, m_s) == m_vn.end())
	{
		return false;
	}

	return std::ranges::all_of(m_p, [&](const auto& rule)
	{
		return HasSRule() && HasNonterminal() && ContainsOnlyVnAndVt(rule.first) && ContainsOnlyVnAndVt(rule.second);
	});

}

bool Grammar::HasSRule()
{
	return std::ranges::any_of(m_p, [this](const auto& rule)
	{
		return rule.first.size() == 1 && rule.first[0] == m_s;
	});
}

bool Grammar::ContainsOnlyVnAndVt(std::vector<std::string> symbols)
{
	return std::ranges::all_of(symbols, [&](const std::string& symbol)
	{
		return std::ranges::find(m_vn, symbol) != m_vn.end() || std::ranges::find(m_vt, symbol) != m_vt.end();
	});
}

bool Grammar::HasNonterminal()
{
	return std::ranges::all_of(m_p, [this](const auto& rule)
	{
		return std::ranges::any_of(rule.first, [this](const auto& input)
		{
			return std::ranges::find(m_vn, input) != m_vn.end();
		});
	});
}

std::ostream& operator<<(std::ostream& out, const Grammar& grammar)
{
	out << "Grammar: G = ({";
	for (const auto& aux : grammar.m_vn)
	{
		out << aux << " , ";
	}
	out << "},{";
	for (const auto& aux : grammar.m_vt)
	{
		out << aux << " , ";
	}
	out << "}," << grammar.m_s << ", P)"
		<< ".\nP containing the following productions:\n";
	for (const auto& [fst, snd] : grammar.m_p)
	{
		for (const auto& aux1 : fst)
		{
			out << aux1;
		}
		out << " -> ";
		for (const auto& aux1 : snd)
		{
			out << aux1;
		}
		out << std::endl;
	}
	return out;
}
std::string Grammar::GetLastWord()
{
	return m_lastWord;
}


void Grammar::GenerateRandomWord( std::string& word, std::ostream& outputStream, const bool printSteps)
{
	if (word == m_s)
		outputStream << "\n" << m_s;
	const std::string producedWord = word;
	m_lastWord = producedWord;
	std::vector<int> applicable_productions;

	for (int i = 0; i < m_p.size(); i++)
	{
		if (producedWord.find(std::accumulate(m_p[i].first.begin(), m_p[i].first.end(), std::string(""))) !=
			std::string::npos)
		{
			applicable_productions.push_back(i);
		}
	}

	if (applicable_productions.empty())
	{
		if (!printSteps)
			outputStream << " => " << word;
		return;
	}
	if (printSteps)
	{
		outputStream << " => ";
	}

	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(0, applicable_productions.size() - 1);
	const int chosen_production = applicable_productions[distr(gen)];

	std::string new_word = ApplyRandomProduction(producedWord, chosen_production);

	if (new_word == producedWord)
	{
		return;
	}
	if (printSteps)
		outputStream << new_word;
	GenerateRandomWord(new_word, outputStream, printSteps);
	word = new_word;
}

std::string Grammar::ApplyRandomProduction(const std::string& input, int production_index)
{
	std::string producedWord = input;
	const std::string left = std::accumulate(m_p[production_index].first.begin(), m_p[production_index].first.end(),
	                                         std::string(""));

	const std::string right = std::accumulate(m_p[production_index].second.begin(), m_p[production_index].second.end(),
	                                          std::string(""));

	if (const int pos = producedWord.find(left); pos != std::string::npos)
	{
		producedWord.replace(pos, left.length(), right);
	}
	return producedWord;
}
