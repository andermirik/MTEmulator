#include "MT.h"
#include "DMT.h"
#include <Windows.h>
void main() {

	/*DMT dmt;
	dmt.readRulesFromFile("kz2.txt");
	for(int i=1;i<=12;i++)
		dmt.combine(i, "output1-12.txt", "points1-12.txt");
		*/
	MT mt;
	mt.readRulesFromFile("kz1.txt");
	mt.generateRandLenta(12, 8);
	//проблемы с расширением ленты

	//первый символ узкое место 24E
		mt.work(true, "________abbb________");
	system("pause");
}