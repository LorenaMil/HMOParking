#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include "car.h"
#include "line.h"

using namespace std;

int numCars = 0;
int numLines = 0;
int lengthCars[100];
int typeCars[100];
int limits[100][100];
int lengthLines[100];
int tripTime[100];
int schedule[100];



struct Block{
	int blockingLine;
	vector<int> blockedLines;
};
vector<Block> blockList;

vector<Car> cars;
vector<Line> lines;

void printBlockList(){
	for (int i = 0; i < blockList.size(); i++){
		cout << blockList[i].blockingLine << ' ';
		for (int j = 0; j < blockList[i].blockedLines.size(); j++){
			cout << blockList[i].blockedLines[j] << ' ';
		}
		cout << '\n';
	}
}
void readFromFile(){
	ifstream file("instanca2.txt");
	int numLine = 0;
	if (file.is_open()){
		string line;

		// number of cars
		getline(file, line);
		numCars = atoi(line.c_str());

		// number of lines
		getline(file, line);
		numLines = atoi(line.c_str());

		// skip a line
		getline(file, line);

		// length of cars - one line
		for (int i = 0; i < numCars; i++){
			file >> lengthCars[i];
		}

		// skip a line
		getline(file, line);

		// type of cars - one line
		for (int i = 0; i < numCars; i++){
			file >> typeCars[i];
		}

		// skip a line
		getline(file, line);

		// limits for cars on specific line
		for (int i = 0; i < numCars; i++){
			for (int j = 0; j < numLines; j++){
				file >> limits[i][j];
			}
		}

		// length of line
		for (int i = 0; i < numLines; i++){
			file >> lengthLines[i];
		}

		// trip time
		for (int i = 0; i < numCars; i++){
			file >> tripTime[i];
		} 

		// type of schedule
		for (int i = 0; i < numCars; i++){
			file >> schedule[i];
		}

		// skip a line
		getline(file, line);
		getline(file, line);

		// blocking list
		while(getline(file, line)){
			Block block;
			//cout << "Line: " << line.c_str() << '\n';
			string part;
			istringstream strLine(line);

			// first read blocking line
			getline(strLine, part, ' ');
			block.blockingLine = atoi(part.c_str());

			while(getline(strLine, part, ' ')){
				(block.blockedLines).push_back(atoi(part.c_str()));
			}
			blockList.push_back(block);
		}
		

//printBlockList();

	}
}

/*
* STARTING INSTANCE - sorts cars by trip time ascending, first fills blocking lines with cars that have earliest trip time
* 	then fills cars with least lines, and then shortest lines
*/
void startingInstance(){
	// create vector of cars
	cars = createCars(numCars, lengthCars, typeCars, tripTime, schedule, numLines, limits);
	//printCars(cars);
	lines = createLines(numLines, lengthLines, cars);
	// sort cars by trip time ascending
	cars = sortCarsByTime(cars);

	cout << "cars 1: " << carsWithType(cars, 1) << " 2: " << carsWithType(cars, 2) << "\n";

	/***** Prepare order for LINES *****/
	// order -> blocking lines, other lines
	vector<int> orderLines;
	// put blocking line ids in the beginning of the vector 
	for (int i = 0; i < blockList.size(); i++){
		orderLines.push_back(blockList[i].blockingLine);
	}

	vector<Line> sortLines = lines;
	sortLines = sortLinesByNum(sortLines);
	// add other lines
	for (int i = 0; i < sortLines.size(); i++){
		int item = sortLines[i].index;
		if (!(find(orderLines.begin(), orderLines.end(), item) != orderLines.end()))
			orderLines.push_back(item);
	}


	// fill lines
	Line line;
	int index = 0;
	int done = 0;
	vector<Car> unsetCars;
	for (int i = 0; i < cars.size(); i++){
		bool set = false;
		Car car = cars[i];

		// strict limits
		for (int j = 0; j < orderLines.size(); j++){
			index = orderLines[j];
			line = lines[index-1]; //starts from 0
			if ((((car.length > 50 && line.length < 90) || (car.length < 50 && line.length > 80) || (usedLength(line) > 40))
				|| (line.allowedCars < numCars-1 && car.allowedLines.size() < numLines-1)) 
				&& ((line.cars.size() == 0 && linesWithType(lines, car.type) < 13) || line.cars.size() > 0)
				&& canAddToLine(line, car)){
				line = addCarToLine(line, car);
				lines.at(index-1) = line;
				done++;
				set = true;
				break;
			}
		}
		// if not set, remove limits and try to set
		if (!set){
			for (int j = 0; j < orderLines.size(); j++){
				index = orderLines[j];
				line = lines[index-1]; //starts from 0
				if (canAddToLine(line, car)){
					line = addCarToLine(line, car);
					lines.at(index-1) = line;
					done++;
					set = true;
					break;
				}
			}

		}

		if (!set) unsetCars.push_back(car);
	}
	printCars(unsetCars);
			printLinesAll(lines);
		cout << done;

		cout << "lines 1: " << linesWithType(lines, 1) << " 2: " << linesWithType(lines, 2) << "\n";

}

//********* FIRST GLOBAL GOAL **************//

// First subgoal
double firstSubgoalF(vector<Line> lines){
	int factor = numUsedLines(lines) - 1;
	int numDiff = numDiffTypes(lines);

	return (double)factor/numDiff;
}

// Second subgoal
double secondSubgoalF(vector<Line> lines){
	int factor = numLines;
	int numL = numUsedLines(lines);

	return (double) numL/factor;
}

// Third subgoal
double thirdSubgoalF(vector<Line> lines){
	double cap = unusedCapacity(lines);
	int factor = totalLinesLength(lines) - totalCarsLength(cars);

	return (double)cap/factor;
}

double firstGoal(vector<Line> lines){
	return (firstSubgoalF(lines) + secondSubgoalF(lines) + thirdSubgoalF(lines));
}


//************ SECOND GLOBAL GOAL *****************//

// First subgoal
double firstSubgoalG(vector<Line> lines){
	int pairs = numSameScheduleInLine(lines);
	int factor = numCars - numUsedLines(lines);

	return (double)pairs/factor;
}

// Second subgoal
double secondSubgoalG(vector<Line> lines){
	int pairs = numSameScheduleLines(lines);
	int factor = numUsedLines(lines) - 1;

	return (double)pairs/factor;
}

// Third subgoal
double thirdSubgoalG(vector<Line> lines){
	int time = timeDiff(lines);
	int factor = 15 * numOfNeighbours(lines);

	return (double)time/factor;
}

double secondGoal(vector<Line> lines){
	return (firstSubgoalG(lines) + secondSubgoalG(lines) + thirdSubgoalG(lines));
}



int main(){
	readFromFile();
	startingInstance();

	return 0;
}