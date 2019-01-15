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

vector<Line> createLines(int numlines, int length[], vector<Car> cars){
	vector<Line> lines;
	for (int i = 0; i < numlines; i++){
		Line line;
		line.index = i+1;
		line.length = length[i];
		line.type = 0;
		line.allowedCars = 0;

		for (int j = 0; j < cars.size(); j++){
			if (allowedLineForCar(line.index, cars[j]))
				line.allowedCars++;
		}

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

void printLinesAll(vector<Line> lines){
	for (int i = 0; i < lines.size(); i++){
		/*cout << lines[i].index << ":\t";
		cout << "Length: " << lines[i].length << ":\t";
		cout << "Used: " << usedLength(lines[i]) << ":\t";
		cout << "Type: " << lines[i].type << ":\t";
		cout << "Allow: " << lines[i].allowedCars << ":\t";*/
		for (int j = 0; j < lines[i].cars.size(); j++){
			cout << lines[i].cars[j].index << " ";
		}
		cout << "\n";
	}
}

void printLinesGoodFormat(vector<Line> lines){
	for (int i = 0; i < lines.size(); i++){
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

int totalLinesLength(vector<Line> lines){
	int total = 0;
	for (int i = 0; i < lines.size(); i++){
		total += lines[i].length;
	}
	return total;
}

// f1 of first global goal
int numDiffTypes(vector<Line> lines){
	int count = 0;
	Line lastLine = lines[0];
	for (int i = 1; i < lines.size(); i++){
		if (lines[i].type == 0) break;
		if (lastLine.type != lines[i].type)
			count++;
		lastLine = lines[i];
	}

	return count;
}


// f2 of first global goal
int numUsedLines(vector<Line> lines){
	int count = 0;
	for (int i = 0; i < lines.size(); i++){
		if (lines[i].cars.size() > 0)
			count++;
	}
	return count;
}


// f3 of first global goal
double unusedCapacity(vector<Line> lines){
	double count = 0.0;
	for (int i = 0; i < lines.size(); i++){
		count += lines[i].length - usedLength(lines[i]);
	}

	return count;
}

// g1 of second global goal
int numSameScheduleInLine(vector<Line> lines){
	int count = 0;
	for (int i = 0; i < lines.size(); i++){
		if (lines[i].cars.size() > 0){
			Car lastCar = lines[i].cars[0];
			for (int j = 1; j < lines[i].cars.size(); j++){
				if (lastCar.schedule == lines[i].cars[j].schedule)
					count++;
			}
		}
	}
	return count;
}


// g2 of second global goal
int numSameScheduleLines(vector<Line> lines){
	int count = 0;
	for (int i = 0; i < lines.size()-1; i++){
		if (lines[i].cars.size() == 0) break;
		int indLast = lines[i].cars.size() - 1;
		for (int j = i+1; j < lines.size(); j++){
			if (lines[j].cars.size() == 0) break;
			if (lines[i].cars[indLast].schedule == lines[j].cars[0].schedule)
				count++;
		}
	}
	return count;
}

// g3 of second global goal
int timeDiff(vector<Line> lines){
	int count = 0;
	for (int i = 0; i < lines.size(); i++){
		if (lines[i].cars.size() < 2) break;
		for (int j = 0; j < lines[i].cars.size()-1; j++){
			int diffTime = lines[i].cars[j+1].time - lines[i].cars[j].time;
			// time = [10,20] n = 15
			if (diffTime >= 10 && diffTime <= 20)
				count += 15;
			// time = <20, +00> n = 10
			else if (diffTime > 20)
				count += 10;
			// time = <-00, 10> n = -4*(10 - time)
			else 
				count += -4 * (10 - diffTime);
		}
	}
	return count;
}

int numOfNeighbours(vector<Line> lines){
	int count = 0;
	for (int i = 0; i < lines.size(); i++){
		if (lines[i].cars.size() < 2) break;
		count += lines[i].cars.size()-1;
	}
	return count;
}

vector<Line> sortLinesByNum(vector<Line> lines){
	sort(lines.begin(), lines.end());
	return lines;
}

bool lineHasCarWithID(Line line, int id){
	for (int i = 0; i < line.cars.size(); i++){
		if (line.cars[i].index == id) 
			return true;
	}
	return false;
}

int linesWithType(vector<Line> lines, int type){
	int count = 0;
	for (int i = 0; i < lines.size(); i++){
		if (lines[i].type == type) count++;
	}
	return count;
}

