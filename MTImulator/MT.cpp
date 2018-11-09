#include "MT.h"
#include <fstream>
#include "Windows.h"
#include <ctime>
MT::MT() {
	lenta = "";
	alphabet = "";
	rules = nullptr;
	count_rules = 0;
	count_spaces = 1;
}

void MT::addRule(size_t condition, char read, size_t condition_next, char write, Command command) {
	Rule rule = { condition, read, condition_next, write, command };
	
	Rule*new_rules = new Rule[count_rules + 1];
	for (int i = 0; i < count_rules; i++) {
		new_rules[i] = rules[i];
	}
	new_rules[count_rules] = rule;

	rules = new_rules;
	count_rules++;

}

bool MT::ruleIsExist(size_t condition, char read, size_t condition_next, char write, Command command)
{
	for (int i = 0; i < count_rules; i++) {
		Rule r = rules[i];
		if (r.command == command && r.condition == condition && 
			r.condition_next == condition_next &&
			r.read == read && r.write == write)
			return true;
	}
	return false;
}

void MT::generateRandLenta(int length, int numSpaces)
{
	lenta = "";
	srand(time(0));

	for (int i = 0; i < numSpaces; i++)
		lenta.push_back('_');
	for (int i = 0; i < length;i++) {
		lenta.push_back(randLetterFromAlphabet());
	}
	for (int i = 0; i < numSpaces; i++) {
		lenta.push_back('_');
	}
	count_spaces = numSpaces;
}

char MT::randLetterFromAlphabet()
{
	
	if(alphabet.size())
		return alphabet[rand()%alphabet.size()];
	else return '_';
}

void MT::setLenta(std::string lenta)
{
	this->lenta = lenta;
}

void MT::setAlphabet(std::string alphabet)
{
	this->alphabet = alphabet;
}

bool MT::readRulesFromFile(std::string filename)
{
	std::ifstream fin;
	fin.exceptions(std::ifstream::failbit);
	fin.exceptions(std::ifstream::badbit);
	try {
		fin.open(filename);
		int tempCondition;
		char tempRead;
		int tempCondition_next;
		char tempWrite;
		char tempCommand;
		while (!fin.eof()) {
			fin >>
				tempCondition >>
				tempRead >>
				tempCondition_next >>
				tempWrite >>
				tempCommand;
			if(!ruleIsExist(tempCondition, tempRead, tempCondition_next, tempWrite, (Command)tempCommand))
				addRule(tempCondition, tempRead, tempCondition_next, tempWrite, (Command)tempCommand);
		}
		fin.close();
	}
	catch (std::exception const&e) {
		std::cerr << "не удалось считать из файла " + filename<<std::endl;
		return false;
	}
	return true;
}

void MT::printLenta()
{
	std::cout << lenta << std::endl;
}

void MT::printRules()
{
	for (int i = 0; i < count_rules; i++) {
		
		std::cout << rules[i].condition << " " << rules[i].read << " " << rules[i].condition_next << " " << rules[i].write << " ";
		switch (rules[i].command)
		{
		case L:
			std::cout << "L";
			break;
		case E:
			std::cout << "E";
			break;
		case R:
			std::cout << "R";
			break;
		}
	std::cout << "\n";
	}
}

bool MT::work(std::string lenta4)
{
	if (lenta4 != "")
		lenta = lenta4;
	
	int currpos = count_spaces;
	int currcond = 0;
	while (true) {
		if(currpos<lenta.size()&&currpos>=0){
			std::cout <<"q"<<currcond;
			Rule r = getNextState(currcond, lenta[currpos]);
			currcond = r.condition_next;
			lenta[currpos] = r.write;

			switch (r.command)
			{
			case L:
				currpos -= 1;
				std::cout << "L:";
				break;
			case E:
				currpos = currpos;
				std::cout << "E:";
				break;
			case R:
				currpos += 1;
				std::cout << "R:";
				break;
			}
			std::cout << "(" << currpos << ")";
			printLenta();
			if (currcond == -1)
				return true;
		}
		else {
			return false;
		}
	}
}

Rule MT::getNextState(int currentState, char currentSymbol)
{
	for (int i = 0; i < count_rules; i++) {
		if (currentState == rules[i].condition && currentSymbol == rules[i].read)
			return rules[i];
	}
}

MT::~MT() {
	delete rules;
}

//Q0 1 ->Q1 1 R
//01 