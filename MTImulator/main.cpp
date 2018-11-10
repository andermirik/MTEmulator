#include "MT.h"
#include "DMT.h"
#include <Windows.h>
#include <fstream>
#include <vector>


std::vector <std::pair<double, double>> getPointsFromFile(std::string filename) {
	std::vector<std::pair<int,int>> points;
	std::ifstream fin(filename);
	int y, x;
	while (!fin.eof()) {
		fin >> y >> x;
		points.push_back(std::pair<int, int>(y, x));
	}
	//нормализация данных
	std::vector<std::pair<double, double>> normalPoints;
	double aveX = 0, Y =0, n=0;
	Y = points[0].first;
	for (auto& it : points) {
		if (it.first == Y) {
			aveX += it.second;
			n++;
		}
		else {
			normalPoints.push_back(std::pair<double, double>(Y, aveX / n));
			Y = it.first;
			aveX += it.second;
			n++;
		}
	}
	normalPoints.push_back(std::pair<double, double>(Y, aveX / n));

	return normalPoints;
}

void main() {

	/*DMT dmt;
	dmt.readRulesFromFile("kz2.txt");
	for(int i=1;i<=12;i++)
		dmt.combine(i, "output1-12.txt", "points1-12.txt");
		*/
	/*
	MT mt;
	mt.readRulesFromFile("kz1.txt");
	mt.generateRandLenta(12, 5);

	mt.work(true, "_____abababab_____");
	std::ofstream o("MT/outputMT1-8.txt");
	o.close();
	o.open("MT/pointsMT1-8.txt");
	o.close();

	for(int i=1;i<=8;i++)
		mt.combine(i, "MT/outputMT1-8.txt", "MT/pointsMT1-8.txt");
		*/

	getPointsFromFile("MT/pointsMT1-8.txt");

	system("pause");
}