#include "Grammar.h"

Grammar::Grammar(std::ifstream& input)
{
	
	std::string aux;
	input >> aux;
	std::istringstream NonSplitStream(aux);
	m_vn = CreateMultime(NonSplitStream);
	aux.erase();
	input >> aux;
	NonSplitStream.str(aux);
	m_vt = CreateMultime(NonSplitStream);
	aux.erase();
	input >> m_s;
	while (!input.eof())
	{


	}
}

std::vector<std::string> Grammar::CreateMultime(std::istringstream& NonSplitStream)
{
	std::vector<std::string> auxVector;
	std::string word;
	while (std::getline(NonSplitStream, word, ','))
	{
		auxVector.push_back(word);
	}
	return auxVector;

}

std::vector<std::pair<std::string, std::string>>& Grammar::CreateVectorP(std::ifstream& input)
{
	std::vector<std::pair<std::string, std::string>> auxP;
	while(!input.eof())
	{
		std::string aux;
		input >> aux;
		std::istringstream NonSplitRule(aux);
		const uint8_t spacePos = aux.find_first_of(' ');
		if (spacePos != std::string::npos)
		{
			auxP.push_back({ aux.substr(0,spacePos),aux.substr(spacePos + 1) });
		}
	}

	return auxP;
}

