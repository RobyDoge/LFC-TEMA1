#pragma once
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iostream>
class Grammar
{
public:
	Grammar();
	Grammar(std::ifstream& input);


private:
	std::vector<std::string> CreateMultime(std::istringstream& NonSplitStream);
	std::vector<std::pair<std::string, std::string>>& CreateVectorP(std::ifstream& input);

	std::vector<std::string> m_vn;
	std::vector<std::string> m_vt;
	std::string m_s;
	std::vector<std::pair<std::string, std::string>> m_p;
};

