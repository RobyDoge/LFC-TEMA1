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

	std::vector<std::string> GetVn();
	std::vector<std::string> GetVt();
	std::string GetS();
	std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> GetP();
	bool IsRegular();

	void GenerateRandomWord(const std::string& word,std::ostream& outputStream,bool printSteps);
	friend std::ostream& operator<<(std::ostream& out, Grammar grammar);


private:
	std::string ApplyRandomProduction(const std::string& input, int production_index);
	std::vector<std::string> SepareteBySpaces(std::istringstream& NonSplitStream);
	std::vector<std::string> SepareteByComma(std::istringstream& NonSplitStream);
	std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> CreateVectorP(std::ifstream& input);
	bool ValidateGrammar();
	
	std::vector<std::string> m_vn;
	std::vector<std::string> m_vt;
	std::string m_s;
	std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> m_p;
};



