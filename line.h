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
	int allowedCars; // how much cars are allowed to be put here
	vector<Car> cars;

	bool operator < (const Line& line) const
    {
        return (allowedCars < line.allowedCars);
    }
};

vector<Line> createLines(int numlines, int length[], vector<Car> cars);

double usedLength(Line line);

bool canAddToLine(Line line, Car car);

Line addCarToLine(Line line, Car car);

Line getLine(vector<Line> lines, int index);

void printLines(vector<Line> lines);

void printLinesAll(vector<Line> lines);

void printLinesGoodFormat(vector<Line> lines);

void printLinesUsage(vector<Line> lines);

int findIndexVector(vector<Line> lines, int indexLine);

int totalLinesLength(vector<Line> lines);

int numDiffTypes(vector<Line> lines);

int numUsedLines(vector<Line> lines);

double unusedCapacity(vector<Line> lines);

int numSameScheduleInLine(vector<Line> lines);

int numSameScheduleLines(vector<Line> lines);

int timeDiff(vector<Line> lines);

int numOfNeighbours(vector<Line> lines);

vector<Line> sortLinesByNum(vector<Line> lines);

bool lineHasCarWithID(Line line, int id);

int linesWithType(vector<Line> lines, int type);

#endif