#include "MT.h"
#include <Windows.h>
void main() {
	MT mt;
	mt.readRulesFromFile("syscmd2.txt");
	mt.setAlphabet("123");
	mt.printLenta();
	mt.printRules();
	//mt.generateRandLenta(9, 1);
	mt.work("_1+11+111+1111V111_");
	//mt.work();
	system("pause");
}