#include "DMT.h"
#include <fstream>
#include <ctime>
DMT::DMT()
{
	lenta = "";
	lenta2 = "";
	alphabet = "";
	rules = nullptr;
	count_rules = 0;
	count_spaces = 1;
}

void DMT::addRule(int condition, char read, char read2, int condition_next, char write, char write2, Command command, Command command2)
{
	Rule rule = { condition, read, read2, condition_next, write, write2, command, command2 };

	Rule*new_rules = new Rule[count_rules + 1];
	for (int i = 0; i < count_rules; i++) {
		new_rules[i] = rules[i];
	}
	new_rules[count_rules] = rule;

	rules = new_rules;
	count_rules++;
}

bool DMT::ruleIsExist(int condition, char read, char read2, int condition_next, char write, char write2, Command command, Command command2)
{
		for (int i = 0; i < count_rules; i++) {
			Rule r = rules[i];
			if (r.command == command && r.condition == condition &&
				r.condition_next == condition_next &&
				r.read == read && r.write == write &&
				r.read2==read2 && r.write2== write2
				)
				return true;
		}
		return false;
}

void DMT::setLenta(std::string lenta)
{
	this->lenta = lenta;
}

void DMT::setAlphabet(std::string alphabet)
{
	this->alphabet = alphabet;
}

bool DMT::readRulesFromFile(std::string filename)
{
	std::ifstream fin;
	fin.exceptions(std::ifstream::failbit);
	fin.exceptions(std::ifstream::badbit);
	try {
		fin.open(filename);
		int tempCondition;
		char tempRead;
		char tempRead2;
		int tempCondition_next;
		char tempWrite;
		char tempWrite2;
		char tempCommand;
		char tempCommand2;
		while (!fin.eof()) {
			fin >>
				tempCondition >>
				tempRead >>
				tempRead2 >>
				tempCondition_next >>
				tempWrite >>
				tempWrite2 >>
				tempCommand >>
				tempCommand2;

			if (!ruleIsExist(tempCondition, tempRead,tempRead2, tempCondition_next, tempWrite,tempWrite2, (Command)tempCommand, (Command)tempCommand2))
				addRule(tempCondition, tempRead, tempRead2, tempCondition_next, tempWrite, tempWrite2, (Command)tempCommand, (Command)tempCommand2);
		}
		fin.close();
	}
	catch (std::exception const&e) {
		std::cerr << "не удалось считать из файла " + filename << std::endl;
		return false;
	}
	return true;
}

void DMT::printLenta()
{
	std::cout << lenta << std::endl;
}

void DMT::printRules()
{
	for (int i = 0; i < count_rules; i++) {

		std::cout << rules[i].condition << " " << rules[i].read<<" "<<rules[i].read2 << " " << rules[i].condition_next << " " << rules[i].write<<" "<<rules[i].write2 << " ";
		switch (rules[i].command)
		{
		case L:
			std::cout << "L ";
			break;
		case E:
			std::cout << "E ";
			break;
		case R:
			std::cout << "R ";
			break;
		}
		switch (rules[i].command2)
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

void DMT::generateRandLenta(int length, int numSpaces)
{
	lenta = "";
	srand(time(0));

	for (int i = 0; i < numSpaces; i++) {
		lenta.push_back('_');
		lenta2.push_back('_');
	}
	for (int i = 0; i < length; i++) {
		lenta.push_back(randLetterFromAlphabet());
		lenta2.push_back('_');
	}
	for (int i = 0; i < numSpaces; i++) {
		lenta.push_back('_');
		lenta2.push_back('_');
	}
	count_spaces = numSpaces;
}

char DMT::randLetterFromAlphabet()
{
	if(alphabet.size())
		return alphabet[rand() % alphabet.size()];
	else return '_';
}

bool DMT::work(bool trace, std::string lenta4)
{
	if (lenta4 != "") {
		lenta = lenta4;
		lenta2.clear();
		for (int i = 0; i < lenta.size(); i++)
			lenta2.push_back('_');
	}

	int currpos = count_spaces;
	int currpos2 = count_spaces;
	int currcond = 0;
	while (true) {
		if (currpos < lenta.size() && currpos >= 0 && currpos2 < lenta.size() && currpos >= 0) {
			if (trace)
				std::cout << "q" << currcond;
			Rule r = getNextState(currcond, lenta[currpos], lenta2[currpos2]);
			currcond = r.condition_next;
			lenta[currpos] = r.write;
			lenta2[currpos2] = r.write2;
			if (trace)
				std::cout << "(" << currpos << ")";
			switch (r.command)
			{
			case L:
				currpos -= 1;
				if (trace)
					std::cout << "L:";
				break;
			case E:
				currpos = currpos;
				if (trace)
					std::cout << "E:";
				break;
			case R:
				currpos += 1;
				if (trace)
					std::cout << "R:";
				break;
			}
			if (trace) {
				printLenta();

			}
			if (trace)
				std::cout << "(" << currpos2 << ")";
			switch (r.command2)
			{
			case L:
				currpos2 -= 1;
				if (trace)
					std::cout << "L:";
				break;
			case E:
				currpos2 = currpos2;
				if (trace)
					std::cout << "E:";
				break;
			case R:
				currpos2 += 1;
				if (trace)
					std::cout << "R:";
				break;
			}
			if (trace) {
				std::cout << "   " << lenta2;
				std::cout << std::endl; std::cout << std::endl;
			}
			if (currcond == -1)
				return true;
		}
		else {
			return false;
		}
	}
}



Rule DMT::getNextState(int currentState, char currentSymbol, char currentSymbol2)
{
	for (int i = 0; i < count_rules; i++) {
		if (currentState == rules[i].condition && currentSymbol == rules[i].read && currentSymbol2 == rules[i].read2)
			return rules[i];
	}
}

DMT::~DMT()
{
	delete[] rules;
}
