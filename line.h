#ifndef LINE_H
#define LINE_H

#include "car.h"
#include <string>
#include <vector>
#include <stdlib.h>
#include <iostream>

using namespace std;

struct Line{
	int index;
	int length;
	int type;
	vector<Car> cars;

	/*bool operator < (const Line& line) const
    {
        return (index < line.index);
    }*/
};

vector<Line> createLines(int numlines, int length[]);

double usedLength(Line line);

bool canAddToLine(Line line, Car car);

Line addCarToLine(Line line, Car car);

Line getLine(vector<Line> lines, int index);

void printLines(vector<Line> lines);

void printLinesUsage(vector<Line> lines);

int findIndexVector(vector<Line> lines, int indexLine);






#endif