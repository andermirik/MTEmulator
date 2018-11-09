#include "DMT.h"
#include <Windows.h>
void main() {

	DMT dmt;
	dmt.readRulesFromFile("kz2.txt");
	for(int i=1;i<=12;i++)
		dmt.combine(i, "output1-12.txt", "points1-12.txt");
	system("pause");
}