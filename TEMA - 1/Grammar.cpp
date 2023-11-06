#include "Grammar.h"
#include <numeric>
#include <random>

Grammar::Grammar(std::ifstream& input)
{
	//reads the first line and puts the elements separted by space in m_vn
	std::string aux;
	std::getline(input, aux);
	std::istringstream NonSplitStream(aux);
	m_vn = SepareteBySpaces(NonSplitStream);
	std::getline(input, aux);

	//clears and resets the istringstream 
	NonSplitStream.clear();
	NonSplitStream.seekg(0);

	//reads the second line and puts the elements separted by space in m_vt
	NonSplitStream.str(aux);
	m_vt = SepareteBySpaces(NonSplitStream);

	//reads the third line and writes it in m_s
	std::getline(input, m_s);

	//reads the lines until the end of the file and puts the pair of rules in m_p
	m_p = CreateVectorP(input);
	input.close();


	if (!ValidateGrammar())
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

bool Grammar::IsRegular()
{
	for (const auto& rule : m_p)
	{
		if (rule.second.size() > 2)
		{
			return false;
		}
		if (rule.first.size() > 1)
		{
			return false;
		}
		if (std::find(m_vt.begin(), m_vt.begin(), rule.first[0])!=m_vt.end())
		{
			return false;
		}
		if (std::find(m_vn.begin(), m_vn.begin(), rule.second[0]) != m_vn.end())
		{
			return false;
		}
		if (rule.second.size() == 2 && std::find(m_vt.begin(), m_vt.begin(), rule.second[1]) != m_vt.end())
		{
			return false;
		}
	}
	return true;
}

//it separets a line into words separated by spaces and puts the words into a vector
std::vector<std::string> Grammar::SepareteBySpaces(std::istringstream& NonSplitStream)
{
	std::vector<std::string> auxVector;
	std::string word;
	while (NonSplitStream >> word)
	{
		auxVector.push_back(word);
	}
	return auxVector;

}

//it separets a line into words separated by commas and puts the words into a vector
std::vector<std::string> Grammar::SepareteByComma(std::istringstream& NonSplitStream)
{
	std::vector<std::string> auxVector;
	std::string word;
	while (std::getline(NonSplitStream, word, ','))
	{
		auxVector.push_back(word);
	}
	return auxVector;

}

//it creats a pair out of the two elements of a line and deposits them in a vector
std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> Grammar::CreateVectorP(std::ifstream& input)
{
	std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> auxP;
	while (!input.eof())
	{
		//Reads the first half of the rule
		std::string NonSplitHalfOfRule;
		input >> NonSplitHalfOfRule;
		std::istringstream NonSplitHalfOfRuleStream(NonSplitHalfOfRule);

		//aux1 is a vector of all the elements in the first half on the rule
		std::vector<std::string> aux1 = SepareteByComma(NonSplitHalfOfRuleStream);

		//reads the second half of the rule
		input >> NonSplitHalfOfRule;

		//adds the new rule in the stream for splitting
		NonSplitHalfOfRuleStream.clear();
		NonSplitHalfOfRuleStream.str(NonSplitHalfOfRule);

		//aux2 is a vector of all the elements in the second half on the rule
		std::vector<std::string> aux2 = SepareteByComma(NonSplitHalfOfRuleStream);

		auxP.push_back({ aux1,aux2 });
	}

	return auxP;
}

bool Grammar::ValidateGrammar()
{
	// Rule 1: VN intersected with VT must be an empty set
	for (const std::string& vn : m_vn) 
	{
		if (std::find(m_vt.begin(), m_vt.end(), vn) != m_vt.end()) 
		{
			return false;
		}
	}

	// Rule 2: S must be in VN
	if (std::find(m_vn.begin(), m_vn.end(), m_s) == m_vn.end()) 
	{
		return false;
	}

	// Rule 3: For each rule, the left side must contain at least one nonterminal
	for (const auto& rule : m_p) 
	{
		bool hasNonterminal = false;
		for (const auto& c : rule.first) 
		{
			if (std::find(m_vn.begin(), m_vn.end(), c) != m_vn.end()) 
			{
				hasNonterminal = true;
				break;
			}
		}
		if (!hasNonterminal) 
		{
			return false;
		}
	}

	// Rule 4: There must be at least one rule that has only S on the left-hand side
	bool hasSRule = false;
	for (const auto& rule : m_p) 
	{
		if (rule.first.size() == 1 && rule.first[0] == m_s) 
		{
			hasSRule = true;
			break;
		}
	}
	if (!hasSRule) 
	{
		return false;
	}

	// Rule 5: Every rule must contain only elements from VN and VT
	for (const auto& rule : m_p) 
	{
		for (const auto& c : rule.second) 
		{
			if (std::find(m_vn.begin(), m_vn.end(), c) == m_vn.end() &&
				std::find(m_vt.begin(), m_vt.end(), c) == m_vt.end()) {
				return false;
			}
		}
		for (const auto& c : rule.first) {
			if (std::find(m_vn.begin(), m_vn.end(), c) == m_vn.end() &&
				std::find(m_vt.begin(), m_vt.end(), c) == m_vt.end()) {
				return false;
			}
		}
	}
}

std::ostream& operator<<(std::ostream& out, Grammar grammar)
{
	out << "Grammar: G = ({";
	for (const auto& aux : grammar.m_vn)
	{
		out << aux << " ";
	}
	out << "},{";
	for (const auto& aux : grammar.m_vt)
	{
		out << aux << " ";
	}
	out << "}," << grammar.m_s << ", P)";
	out << " P containing the following productions:\n";
	for (const auto& aux : grammar.m_p)
	{
		for (const auto& aux1 : aux.first)
		{
			out << aux1 << " ";
		}
		out << "-> ";
		for (const auto& aux1 : aux.second)
		{
			out << aux1;
		}
		out << std::endl;
	}
	return out;
}


void Grammar::GenerateRandomWord(const std::string& word, std::ostream& outputStream, bool printSteps)
{
	if (word == m_s)
		outputStream << m_s;
	std::string producedWord = word;
	std::vector<int> applicable_productions;

	// Check if one of the rules from the left side is applicable for the producedWord 
	// Because we used a std::vector<std::string>> ,used the accumulate funtion to make the vector into only one string

	for (int i = 0; i < m_p.size(); i++) 
	{
		if (producedWord.find(std::accumulate(m_p[i].first.begin(), m_p[i].first.end(), std::string(""))) != std::string::npos) {
			applicable_productions.push_back(i);
		}
	}

	// If there are no applicable functions return from the recursive function
	if (applicable_productions.empty()) 
	{
		if (!printSteps)
			outputStream << " => " << word;
		return;
	}
	else if(printSteps)
	{
		outputStream << " => ";
	}

	// Generate a random index to use for a production
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> distr(0, applicable_productions.size() - 1);
	int chosen_production = applicable_productions[distr(gen)];

	//Apply the random production to the producedWord
	std::string new_word = ApplyRandomProduction(producedWord, chosen_production);

	//This should not really happen but just for safety so we don`t have an infinite loop
	if (new_word == producedWord) 
	{
		return;
	}
	if (printSteps)
		outputStream << new_word;
	GenerateRandomWord(new_word,outputStream,printSteps);
	
}

std::string Grammar::ApplyRandomProduction(const std::string& input, int production_index) 
{
	std::string producedWord = input;
	std::string left = std::accumulate(m_p[production_index].first.begin(), m_p[production_index].first.end(), std::string(""));
	std::string right;
	if (m_p[production_index].second[0] == "lambda")
	{
		right = "";
	}
	else
	{
		right = std::accumulate(m_p[production_index].second.begin(), m_p[production_index].second.end(), std::string(""));
	}

	//Search the position of the left side of the production in the producedWord
	int pos = producedWord.find(left);

	//If the position is valid , replace the left side with the right side 
	if (pos != std::string::npos) 
	{
		producedWord.replace(pos, left.length(), right);
	}
	return producedWord;
}