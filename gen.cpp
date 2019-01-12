#ifndef GEN
#define GEN


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


vector<Block> block_list(){
	return blockList;
}

void readFromFile(){
	ifstream file("instanca3.txt");
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
* 	then fills other lines
*/
void startingInstance(){
	// create vector of cars
	cars = createCars(numCars, lengthCars, typeCars, tripTime, schedule, numLines, limits);
	//printCars(cars);
	lines = createLines(numLines, lengthLines);
	// sort cars by trip time ascending
	cars = sortCarsByTime(cars);

	/***** Prepare order for LINES *****/
	// order -> blocking lines, other lines
	vector<int> orderLines;
	// put blocking line ids in the beginning of the vector 
	for (int i = 0; i < blockList.size(); i++){
		orderLines.push_back(blockList[i].blockingLine);
	}
	// add other lines
	for (int i = 0; i < numLines; i++){
		int item = i + 1;
		if (!(find(orderLines.begin(), orderLines.end(), item) != orderLines.end()))
			orderLines.push_back(item);
	}

	// fill lines
	int index = 0;
	int did = 0;
	//Line line;
	for (int i = 0; i < cars.size(); i++){
		Car car = cars[i];
		for (int j = 0; j < orderLines.size(); j++){
			index = orderLines[j];
			auto line = lines[index];
			if (canAddToLine(line, car)){
				line = addCarToLine(line, car);
				int id = findIndexVector(lines, index);
				lines[id] = line;
				did++;
				break;
			}
		}
	}
	printLinesUsage(lines);
	printLines(lines);
	cout << did;

}

//********* FIRST GLOBAL GOAL **************//

// First subgoal
double firstSubgoalF(const vector<Line> & lines){
	int factor = numUsedLines(lines) - 1;
	int numDiff = numDiffTypes(lines);

	return (double)factor/numDiff;
}

// Second subgoal
double secondSubgoalF(const vector<Line> & lines){
	int factor = numLines;
	int numL = numUsedLines(lines);

	return (double) numL/factor;
}

// Third subgoal
double thirdSubgoalF(const vector<Line> & lines){
	double cap = unusedCapacity(lines);
	int factor = totalLinesLength(lines) - totalCarsLength(cars);

	return (double)cap/factor;
}

double firstGoal(const vector<Line> & lines){
	return (firstSubgoalF(lines) + secondSubgoalF(lines) + thirdSubgoalF(lines));
}


//************ SECOND GLOBAL GOAL *****************//

// First subgoal
double firstSubgoalG(const vector<Line> & lines){
	int pairs = numSameScheduleInLine(lines);
	int factor = numCars - numUsedLines(lines);

	return (double)pairs/factor;
}

// Second subgoal
double secondSubgoalG(const vector<Line> & lines){
	int pairs = numSameScheduleLines(lines);
	int factor = numUsedLines(lines) - 1;

	return (double)pairs/factor;
}

// Third subgoal
double thirdSubgoalG(const vector<Line> & lines){
	int time = timeDiff(lines);
	int factor = 15 * numOfNeighbours(lines);

	return (double)time/factor;
}

double secondGoal(const vector<Line> & lines){
	return (firstSubgoalG(lines) + secondSubgoalG(lines) + thirdSubgoalG(lines));
}



int main(){
	readFromFile();
	startingInstance();

	return 0;
}
#endif // !1