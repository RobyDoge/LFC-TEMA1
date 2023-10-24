#include "Grammar.h"

Grammar::Grammar(std::ifstream& input)
{
	//reads the first line and puts the elements separted by space in m_vn
	std::string aux;
	std::getline(input,aux);
	std::istringstream NonSplitStream(aux);
	m_vn = CreateMultime(NonSplitStream);
	std::getline(input, aux);
	
	//clears and resets the istringstream 
	NonSplitStream.clear();
	NonSplitStream.seekg(0);

	//reads the second line and puts the elements separted by space in m_vt
	NonSplitStream.str(aux);
	m_vt = CreateMultime(NonSplitStream);

	//reads the third line and writes it in m_s
	std::getline(input ,m_s);

	//reads the lines until the end of the file and puts the pair of rules in m_p
	m_p = CreateVectorP(input);
	input.close();
}

//it separets a line into words separated my spaces and puts the words in a vector
std::vector<std::string> Grammar::CreateMultime(std::istringstream& NonSplitStream)
{
	std::vector<std::string> auxVector;
	std::string word;
	while (NonSplitStream>>word)
	{
		auxVector.push_back(word);
	}
	return auxVector;

}

//it creats a pair out of the two elements of a line and deposits them in a vector
std::vector<std::pair<std::string, std::string>> Grammar::CreateVectorP(std::ifstream& input)
{
	std::vector<std::pair<std::string, std::string>> auxP;
	while(!input.eof())
	{
		std::string NonSplitRule;
		std::getline(input, NonSplitRule);
		std::istringstream NonSplitRuleStream(NonSplitRule);
		std::string aux1,aux2;
		NonSplitRuleStream >> aux1 >> aux2;
		auxP.push_back({ aux1,aux2});
		
	}

	return auxP;
}

bool Grammar::validateGrammar()
{
	// Rule 1: VN intersected with VT must be an empty set
	for (const std::string& vn : m_vn) {
		if (std::find(m_vt.begin(), m_vt.end(), vn) != m_vt.end()) {
			return false;
		}
	}

	// Rule 2: S must be in VN
	if (std::find(m_vn.begin(), m_vn.end(), m_s) == m_vn.end()) {
		return false;
	}

	// Rule 3: For each rule, the left side must contain at least one nonterminal
	for (const auto& rule : m_p) {
		bool hasNonterminal = false;
		for (const auto& c : rule.first) {
			if (std::find(m_vn.begin(), m_vn.end(), c) != m_vn.end()) {
				hasNonterminal = true;
				break;
			}
		}
		if (!hasNonterminal) {
			return false;
		}
	}

	// Rule 4: There must be at least one rule that has only S on the left-hand side
	bool hasSRule = false;
	for (const auto& rule : m_p) {
		if (rule.first == m_s) {
			hasSRule = true;
			break;
		}
	}
	if (!hasSRule) {
		return false;
	}

	// Rule 5: Every rule must contain only elements from VN and VT
	for (const auto& rule : m_p) {
		for (const auto& c : rule.second) {
			if (std::find(m_vn.begin(), m_vn.end(), c) == m_vn.end() &&
				std::find(m_vt.begin(), m_vt.end(), c) == m_vt.end()) {
				return false;
			}
		}
	}
}