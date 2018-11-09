#include "DMT.h"
#include <Windows.h>
void main() {
	/*MT mt;
	mt.readRulesFromFile("syscmd2.txt");
	mt.setAlphabet("123");
	//mt.printRules();
	//mt.generateRandLenta(9, 1);
	mt.printLenta();
	mt.work(false, "_1+11+111+1111V111_");
	mt.printLenta();*/

	DMT dmt;
	dmt.readRulesFromFile("dsyscmd.txt");
	dmt.setAlphabet("ab");
	dmt.generateRandLenta(9,1);
	dmt.printLenta();
	dmt.work(true);
	system("pause");
}