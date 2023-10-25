#include "Grammar.h"

Grammar::Grammar(std::ifstream& input)
{
	//reads the first line and puts the elements separted by space in m_vn
	std::string aux;
	std::getline(input,aux);
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
	std::getline(input ,m_s);

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

//it separets a line into words separated by spaces and puts the words into a vector
std::vector<std::string> Grammar::SepareteBySpaces(std::istringstream& NonSplitStream)
{
	std::vector<std::string> auxVector;
	std::string word;
	while (NonSplitStream>>word)
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
	while(!input.eof())
	{
		//Reads the first half of the rule
		std::string NonSplitHalfOfRule;
		input>> NonSplitHalfOfRule;
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

		auxP.push_back({ aux1,aux2});
	}

	return auxP;
}

bool Grammar::ValidateGrammar()
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
		if (rule.first.size()==1 && rule.first[0]==m_s) {
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

std::ostream& operator<<(std::ostream& out, Grammar grammar)
{
	out << "Multimea elementelor Neterminale sunt:\n";
	for (const auto& aux : grammar.m_vn)
	{
		out << aux << " ";
	}
	out << "\n\nMultimea elementelor Termianle sunt:\n";
	for (const auto& aux : grammar.m_vt)
	{
		out << aux << " ";
	}
	out << "\n\nElementul de start este: "<<grammar.m_s<<std::endl;
	out << "\nMultimea regurilor sunt:\n";
	for (const auto& aux : grammar.m_p)
	{
		for (const auto& aux1 : aux.first)
		{
			out << aux1 << " ";
		}
		out << "-> ";
		for (const auto& aux1 : aux.second)
		{
			out << aux1 << " ";
		}
		out << std::endl;
	}
	return out;
}
