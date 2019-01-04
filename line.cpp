#include "line.h"
#include <string>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <algorithm>

using namespace std;
/*
struct Line{
	int index;
	int length;
	int type;
	vector<Car> cars;
};*/

vector<Line> createLines(int numlines, int length[]){
	vector<Line> lines;
	for (int i = 0; i < numlines; i++){
		Line line;
		line.index = i+1;
		line.length = length[i];
		line.type = 0;

		lines.push_back(line);
	}
	return lines;
}

double usedLength(Line line){
	double sum = 0.0;
	for (int i = 0; i < line.cars.size(); i++){
		sum += line.cars[i].length;
	}
	if (line.cars.size() >= 2) sum += (line.cars.size() - 1) * 0.5;
	return sum;
}

bool canAddToLine(Line line, Car car){
	if (((double) car.length + usedLength(line) + 0.5 <= line.length)
		&& (allowedLineForCar(line.index, car))
		&& ((line.type == 0) || (line.type == car.type)))
		return true;
	else return false;
}

Line addCarToLine(Line line, Car car){
	line.cars.push_back(car);
	if (line.cars.size() == 1) line.type = car.type;
	line.cars = sortCarsByTime(line.cars);
	return line;
}

Line getLine(vector<Line> lines, int index){
	for (int i = 0; i < lines.size(); i++){
		if (lines[i].index == index){
			return lines[i];
		}
	}
	//return NULL;
}

void printLines(vector<Line> lines){
	for (int i = 0; i < lines.size(); i++){
		cout << lines[i].index << ":\t";
		for (int j = 0; j < lines[i].cars.size(); j++){
			cout << lines[i].cars[j].index << " ";
		}
		cout << "\n";
	}
}

void printLinesUsage(vector<Line> lines){
	for (int i = 0; i < lines.size(); i++){
		cout << lines[i].index << "\t" << lines[i].length << " ";
		cout << usedLength(lines[i]) << "\n";
	}
}

int findIndexVector(vector<Line> lines, int indexLine){
	int indexVector = 0;
	for (int i = 0; i < lines.size(); i++){
		if (lines[i].index == indexLine){
			return i;
		}
	}
	return -1;
}