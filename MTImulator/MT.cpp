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

void MT::addRule(int condition, char read, int condition_next, char write, Command command) {
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

bool MT::work(bool trace, std::string lenta4)
{
	if (lenta4 != "")
		lenta = lenta4;
	
	int currpos = count_spaces;
	int currcond = 0;
	while (true) {
		if(currpos<lenta.size()&&currpos>=0){
			if(trace)
				std::cout <<"q"<<currcond;
			Rule r = getNextState(currcond, lenta[currpos]);
			currcond = r.condition_next;
			lenta[currpos] = r.write;

			switch (r.command)
			{
			case L:
				currpos -= 1;
				if(trace)
					std::cout << "L:";
				break;
			case E:
				currpos = currpos;
				if(trace)
					std::cout << "E:";
				break;
			case R:
				currpos += 1;
				if(trace)
					std::cout << "R:";
				break;
			}
			if (trace) {
				std::cout << "(" << currpos << ")";
				printLenta(); std::cout << std::endl;
			}
			if (currcond == -1)
				return true;
		}
		else {
			return false;
		}
	}
}

int MT::workDebugFile(std::string filename, std::string lenta4)
{
	if (lenta4 != "") {
		lenta = lenta4;
	}
	std::ofstream fout(filename, std::ios::app);
	fout << "\ninput lenta: " << lenta << std::endl << std::endl;
	int currpos = count_spaces;
	int currcond = 0;
	int path = 0;
	while (true) {
		if (currpos < lenta.size() && currpos >= 0) {

			fout << "q" << currcond;
			Rule r = getNextState(currcond, lenta[currpos]);
			currcond = r.condition_next;
			lenta[currpos] = r.write;
			fout << "(" << currpos << ")";
			
			switch (r.command)
			{
			case L:
				currpos -= 1;
				path += 1;
				fout << "L:";
				break;
			case E:
				currpos = currpos;
				path = path;
				fout << "E:";
				break;
			case R:
				path += 1;
				currpos += 1;
				fout << "R:";
				break;
			}

			fout << lenta;
			fout << std::endl;

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

void MT::combine(int length, std::string filename, std::string filenamepoints)
{
	//aaa aab aac aba abb abc aca acb acc baa bab bac bba 
	std::string _lenta;
	count_spaces = length / 4 + 1;
	for (int i = 0; i < length + count_spaces * 2; i++) {
		_lenta.push_back('_');
	}
	int i = count_spaces;
	for (; i < length + count_spaces; ++i)
		_lenta[i] = 'a';
	std::ofstream fout(filenamepoints, std::ios::app);
	for (; ; ) {
		std::cout << _lenta << std::endl;
		fout << length << " " << workDebugFile(filename, _lenta) << std::endl;
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

int MT::workForGraphic(std::string lenta4)
{
	if (lenta4 != "") {
		lenta = lenta4;
	}
	int currpos = count_spaces;
	int currcond = 0;
	int path = 0;
	while (true) {
		if (currpos < lenta.size() && currpos >= 0) {

			Rule r = getNextState(currcond, lenta[currpos]);
			currcond = r.condition_next;
			lenta[currpos] = r.write;

			switch (r.command)
			{
			case L:
				currpos -= 1;
				path += 1;
				break;
			case E:
				currpos = currpos;
				path = path;
				break;
			case R:
				path += 1;
				currpos += 1;
				break;
			}
			if (currcond == -1) {
				return path;
			}
		}
		else {
			return path;
		}
	}
}

void MT::combineCreateVectorForGraphic(int length, std::vector<std::pair<int, int>>& tp)
{
	std::string _lenta;
	count_spaces = length / 4 + 2;
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

void MT::pointsForGraphic(int l, int r)
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

Rule MT::getNextState(int currentState, char currentSymbol)
{
	for (int i = 0; i < count_rules; i++) {
		if (currentState == rules[i].condition && currentSymbol == rules[i].read)
			return rules[i];
	}
}

MT::~MT() {
	delete[] rules;
}

//Q0 1 ->Q1 1 R
//01 