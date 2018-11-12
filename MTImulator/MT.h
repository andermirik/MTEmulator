#pragma once
#include <iostream>
#include <string>
#include <vector>
enum Command{L='L', E='E', R='R'};
struct Rule
{
	int condition;
	char read;
	int condition_next;
	char write;
	Command command;
	
};

class MT{
	std::string lenta;
	std::string alphabet;
	int count_spaces;
	Rule*rules;
	size_t count_rules;

public:
	std::vector<std::pair<double, double>> points;
	void addRule(int condition, char read, int condition_next, char write, Command command);
	bool ruleIsExist(size_t condition, char read, size_t condition_next, char write, Command command);
	void generateRandLenta(int length, int numSpaces);
	char randLetterFromAlphabet();
	void setLenta(std::string lenta);
	void setAlphabet(std::string alphabet);
	bool readRulesFromFile(std::string filename);
	void printLenta();
	void printRules();
	bool work(bool trace, std::string lenta4 = "");
	int workDebugFile(std::string filename, std::string lenta4 = "");
	void combine(int n, std::string filename, std::string filenamepoints);
	int workForGraphic(std::string lenta4 = "");
	void combineCreateVectorForGraphic(int n, std::vector<std::pair<int, int>>& tp);
	void pointsForGraphic(int l, int r);
	Rule getNextState(int cst, char csb);
	MT();
	~MT();
};