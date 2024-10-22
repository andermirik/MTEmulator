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

void DMT::addRule(int condition, char read, char read2, int condition_next, char write, char write2, dmt::Command command, dmt::Command command2)
{
	dmt::Rule rule = { condition, read, read2, condition_next, write, write2, command, command2 };

	dmt::Rule*new_rules = new dmt::Rule[count_rules + 1];
	for (int i = 0; i < count_rules; i++) {
		new_rules[i] = rules[i];
	}
	new_rules[count_rules] = rule;

	rules = new_rules;
	count_rules++;
}

bool DMT::ruleIsExist(int condition, char read, char read2, int condition_next, char write, char write2, dmt::Command command, dmt::Command command2)
{
		for (int i = 0; i < count_rules; i++) {
			dmt::Rule r = rules[i];
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

			if (!ruleIsExist(tempCondition, tempRead,tempRead2, tempCondition_next, tempWrite,tempWrite2, (dmt::Command)tempCommand, (dmt::Command)tempCommand2))
				addRule(tempCondition, tempRead, tempRead2, tempCondition_next, tempWrite, tempWrite2, (dmt::Command)tempCommand, (dmt::Command)tempCommand2);
		}
		fin.close();
	}
	catch (std::exception const&e) {
		std::cerr << "�� ������� ������� �� ����� " + filename << std::endl;
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
		case dmt::L:
			std::cout << "L ";
			break;
		case dmt::E:
			std::cout << "E ";
			break;
		case dmt::R:
			std::cout << "R ";
			break;
		}
		switch (rules[i].command2)
		{
		case dmt::L:
			std::cout << "L";
			break;
		case dmt::E:
			std::cout << "E";
			break;
		case dmt::R:
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

int DMT::moveWrap(dmt::Command command, int & pos, bool trace=true)
{
	switch (command)
	{
	case dmt::L:
		pos -= 1;
		if (trace)
			std::cout << "L:";
		return 1;
	case dmt::E:
		pos = pos;
		if (trace)
			std::cout << "E:";
		return 0;
	case dmt::R:
		pos += 1;
		if (trace)
			std::cout << "R:";
		return 1;
	}
}

int DMT::moveWrapFile(dmt::Command command, int & pos, std::ofstream & fout)
{
	switch (command)
	{
	case dmt::L:
		pos -= 1;
		fout << "L:";
		return 1;
	case dmt::E:
		pos = pos;
		fout << "E:";
		return 0;
	case dmt::R:
		pos += 1;
		fout << "R:";
		return 1;
	}
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
		count_spaces = 1;
		lenta.insert(0, count_spaces, '_');
		for (int i = 0; i < count_spaces; i++)
			lenta.push_back('_');
		
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
			dmt::Rule r = getNextState(currcond, lenta[currpos], lenta2[currpos2]);
			currcond = r.condition_next;
			lenta[currpos] = r.write;
			lenta2[currpos2] = r.write2;
			if (trace)
				std::cout << "(" << currpos << ")";
			moveWrap(r.command, currpos);
			if (trace) {
				printLenta();
				std::cout << "(" << currpos2 << ")";
			}
			moveWrap(r.command2, currpos2);
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

int DMT::workDebugFile(std::string filename, std::string lenta4)
{
	if (lenta4 != "") {
		lenta = lenta4;
		lenta2.clear();
		count_spaces = 1;
		lenta.insert(0, count_spaces, '_');
		for (int i = 0; i < count_spaces; i++)
			lenta.push_back('_');

		for (int i = 0; i < lenta.size(); i++)
			lenta2.push_back('_');
	}
	std::ofstream fout(filename, std::ios::app);
	fout << "\ninput lenta: " << lenta << std::endl << std::endl;
	int currpos = count_spaces;
	int currpos2 = count_spaces;
	int currcond = 0;
	int path=0;
	while (true) {
		if (currpos < lenta.size() && currpos >= 0 && currpos2 < lenta.size() && currpos >= 0) {
			
			fout << "q" << currcond;
			dmt::Rule r = getNextState(currcond, lenta[currpos], lenta2[currpos2]);
			currcond = r.condition_next;
			lenta[currpos] = r.write;
			lenta2[currpos2] = r.write2;

			fout << "(" << currpos << ")";
			path+=moveWrapFile(r.command, currpos, fout);
			
			fout << lenta;
			fout << std::endl;

			fout << "(" << currpos2 << ")";
			path+=moveWrapFile(r.command2, currpos2, fout);
			
			fout << "   " << lenta2;
			fout << std::endl; fout << std::endl;

			if (currcond == -1) {
				fout << "output lenta: " << lenta << std::endl;
				fout.close();
				return path;
			}
		}
		else {
			fout.close();
			return path;
		}
	}

}

int DMT::workForGraphic(std::string lenta4)
{
	if (lenta4 != "") {
		lenta = lenta4;
		lenta2.clear();
		/*
		count_spaces = 1;
		lenta.insert(0, count_spaces, '_');
		for (int i = 0; i < count_spaces; i++)
			lenta.push_back('_');
		*/
		for (int i = 0; i < lenta.size(); i++)
			lenta2.push_back('_');
	}

	int currpos = count_spaces;
	int currpos2 = count_spaces;
	int currcond = 0;
	int path = 0;
	while (true) {
		if (currpos < lenta.size() && currpos >= 0 && currpos2 < lenta2.size() && currpos2 >= 0) {

			dmt::Rule r = getNextState(currcond, lenta[currpos], lenta2[currpos2]);
			currcond = r.condition_next;
			lenta[currpos] = r.write;
			lenta2[currpos2] = r.write2;

			path += moveWrap(r.command, currpos, false);

			path += moveWrap(r.command2, currpos2, false);

			if (currcond == -1) {
				return path;
			}
		}
		else {
			return path;
		}
	}
}

void DMT::combineCreateVectorForGraphic(int length, std::vector<std::pair<int, int>>&tp)
{
	
	std::string _lenta;
	for (int i = 0; i < length + count_spaces * 2; i++) {
		_lenta.push_back('_');
	}
	int i = count_spaces;
	for (; i < length + count_spaces; ++i)
		_lenta[i] = 'a';
	for (; ; ) {
		tp.push_back(std::pair<int, int>(length, workForGraphic(_lenta)));
		{
			int pos = length + count_spaces;
			char digit = _lenta[--pos];
			for (; digit == 'c'; digit = _lenta[--pos]) {
				if (pos == count_spaces)
					return;
				_lenta[pos] = 'a';
			}
			_lenta[pos] = ++digit;
		}
	}

}

void DMT::pointsForGraphic(int l, int r)
{
	std::vector<std::pair<int, int>> tp;
	for (int i = l; i <= r; i++) {
		combineCreateVectorForGraphic(i, tp);
	}

	double aveX = 0, Y = 0, n = 0;
	Y = tp[0].first;
	for (auto& it : tp) {
		if (it.first == Y) {
			aveX += it.second;
			n++;
		}
		else {
			points.push_back(std::pair<double, double>(Y, aveX / n));
			Y = it.first;
			aveX += it.second;
			n++;
		}
	}
	points.push_back(std::pair<double, double>(Y, aveX / n));
}

void DMT::combine(int length, std::string filename, std::string filenamepoints)
{
	//aaa aab aac aba abb abc aca acb acc baa bab bac bba 
	std::string _lenta;
	for (int i = 0; i < length+count_spaces*2; i++) {
		_lenta.push_back('_');
	}
	int i = count_spaces;
	for (; i < length+ count_spaces; ++i)
		_lenta[i] = 'a';
	std::ofstream fout(filenamepoints, std::ios::app);
	for (; ; ) {
		std::cout << _lenta << std::endl;
		fout << length<<" "<<workDebugFile(filename, _lenta)<<std::endl;
		{
			int pos = length+ count_spaces;
			char digit = _lenta[--pos];
			for (; digit == 'c'; digit = _lenta[--pos]) {
				if (pos == count_spaces)
					return;
				_lenta[pos] = 'a';
			}
			_lenta[pos] = ++digit;
		}
	}
}



dmt::Rule DMT::getNextState(int currentState, char currentSymbol, char currentSymbol2)
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
