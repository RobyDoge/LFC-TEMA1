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

