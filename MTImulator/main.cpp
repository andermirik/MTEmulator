#include "MT.h"
#include "DMT.h"
#include <Windows.h>
#include <fstream>
void main() {

	/*DMT dmt;
	dmt.readRulesFromFile("kz2.txt");
	for(int i=1;i<=12;i++)
		dmt.combine(i, "output1-12.txt", "points1-12.txt");
		*/
	MT mt;
	mt.readRulesFromFile("kz1.txt");
	mt.generateRandLenta(12, 5);
	//проблемы с расширением ленты

	
		mt.work(true, "_____abababab_____");
	std::ofstream o("MT/outputMT1-8.txt");
	o.close();
	o.open("MT/pointsMT1-8.txt");
	o.close();

	for(int i=1;i<=8;i++)
		mt.combine(i, "MT/outputMT1-8.txt", "MT/pointsMT1-8.txt");
	system("pause");
}