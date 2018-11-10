#pragma once
#include <iostream>
#include <string>
namespace dmt {
	enum Command { L = 'L', E = 'E', R = 'R' };
	struct Rule
	{
		int condition;
		char read;
		char read2;
		int condition_next;
		char write;
		char write2;
		Command command;
		Command command2;
	};
}

class DMT
{
private:
	std::string alphabet;
	std::string lenta;
	std::string lenta2;
	int count_spaces;
	dmt::Rule*rules; 
	size_t count_rules;
public:
	DMT();
	void addRule(int condition, char read, char read2, int condition_next, char write, char write2, dmt::Command command, dmt::Command command2);
	bool ruleIsExist(int condition, char read, char read2, int condition_next, char write, char write2, dmt::Command command, dmt::Command command2);
	void setLenta(std::string lenta);
	void setAlphabet(std::string alphabet);
	bool readRulesFromFile(std::string filename);
	void printLenta();
	void printRules();
	void generateRandLenta(int length, int numSpaces);
	int moveWrap(dmt::Command command, int&pos, bool move);
	int moveWrapFile(dmt::Command command, int&pos, std::ofstream&fout);
	char randLetterFromAlphabet();
	bool work(bool trace, std::string lenta4 = "");
	int workDebugFile(std::string filename, std::string lenta4 = "");
	void combine(int n, std::string filename, std::string filenamepoints);
	dmt::Rule getNextState(int cst, char csb, char csb2);
	~DMT();
};

