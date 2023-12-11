#pragma once
#include <vector>
#include <string>
#include <sstream>

class Grammar
{
public:
	Grammar() = default;				
	Grammar(std::ifstream& input);	

	std::vector<std::string> GetVn();
	std::vector<std::string> GetVt();
	std::string GetS();
	std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> GetP();
	bool GetGrammarCheck();

	bool IsRegular();
	void GenerateRandomWord( std::string& word,std::ostream& outputStream,bool printSteps);

	friend std::ostream& operator<<(std::ostream& out, const Grammar& grammar);
	std::string GetLastWord();
private:
	std::string ApplyRandomProduction(const std::string& input, int production_index);
	static std::vector<std::string> SeparateBySpaces(std::istringstream& nonSplitStream);
	static std::vector<std::string> SeparateByComma(std::istringstream& nonSplitStream);
	std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> CreateVectorP(std::ifstream& input);
	bool VerifyGrammar();
	bool HasSRule();
	bool ContainsOnlyVnAndVt(std::vector<std::string> symbols);
	bool HasNonterminal();

private:
	std::vector<std::string> m_vn;
	std::vector<std::string> m_vt;
	std::string m_s;
	std::vector<std::pair<std::vector<std::string>, std::vector<std::string>>> m_p;
	std::string m_lastWord;
};



