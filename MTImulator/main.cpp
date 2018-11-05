#include "MT.h"
#include <Windows.h>
void main() {
	MT mt;
	mt.readRulesFromFile("syscmd2.txt");
	mt.setAlphabet("1+_V*");
	mt.printLenta();
	mt.printRules();
	mt.work("_1+11+111+1111V111_");	

	system("pause");
}