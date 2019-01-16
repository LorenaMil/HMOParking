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

double usedLength(const Line & line);

bool canAddToLine(const Line & line, const Car & car);

Line addCarToLine(Line line, Car car);

Line getLine(const vector<Line> & lines, int index);

void printLines(vector<Line> lines);

void printLinesAll(vector<Line> lines);

void printLinesGoodFormat(vector<Line> lines);

void printLinesUsage(vector<Line> lines);

int findIndexVector(vector<Line> lines, int indexLine);

int totalLinesLength(const vector<Line> & lines);

int numDiffTypes(const vector<Line> & lines);

int numUsedLines(const vector<Line> & lines);

double unusedCapacity(const vector<Line> & lines);

int numSameScheduleInLine(const vector<Line> & lines);

int numSameScheduleLines(const vector<Line> & lines);

int timeDiff(const vector<Line> & lines);

int numOfNeighbours(const vector<Line> & lines);

vector<Line> sortLinesByNum(vector<Line> lines);

bool lineHasCarWithID(const Line & line, const int & id);

int linesWithType(const vector<Line> & lines, const int & type);

#endif