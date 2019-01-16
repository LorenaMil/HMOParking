#ifndef gen
#define gen


#include <sstream>
#include <string>
#include <vector>
#include <fstream>
#include <stdlib.h>
#include <iostream>
#include <algorithm>
#include "car.h"
#include "line.h"
#include "GeneticAlgorithm.h"
#include "Chromosom.h"
#include <chrono>
#include "goal_function.h"
#include "blockLines.h"
#include "Writer.h"
using namespace std;

int numCars = 0;
int numLines = 0;
int lengthCars[100];
int typeCars[100];
int limits[100][100];
int lengthLines[100];
int tripTime[100];
int schedule[100];




	vector<Block> blockList;

	vector<Car> cars;
	vector<Line> lines;

	struct LineAllow {
		int index;
		int allow;

		bool operator < (const LineAllow& lineAllow) const
		{
			return (allow < lineAllow.allow);
		}
	};
	vector<LineAllow> lineAllow;

	vector<LineAllow> fillLineAllow(vector<int> ind) {
		vector<LineAllow> lls;
		LineAllow ll;
		Line line;
		for (int i = 0; i < ind.size(); i++) {
			line = lines[ind[i] - 1];
			ll.index = line.index;
			ll.allow = line.allowedCars;
			lls.push_back(ll);
		}
		return lls;
	}

	vector<LineAllow> sortLineAllow(vector<LineAllow> lls) {
		sort(lls.begin(), lls.end());
		return lls;
	}

/*	void printBlockList() {
		for (int i = 0; i < blockList.size(); i++) {
			cout << blockList[i].blockingLine << ' ';
			for (int j = 0; j < blockList[i].blockedLines.size(); j++) {
				cout << blockList[i].blockedLines[j] << ' ';
			}
			cout << '\n';
		}
	}
*/

	void readFromFile(char** argv) {
		ifstream file(argv[1]);
		int numLine = 0;
		if (file.is_open()) {
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
			for (int i = 0; i < numCars; i++) {
				file >> lengthCars[i];
			}

			// skip a line
			getline(file, line);

			// type of cars - one line
			for (int i = 0; i < numCars; i++) {
				file >> typeCars[i];
			}

			// skip a line
			getline(file, line);

			// limits for cars on specific line
			for (int i = 0; i < numCars; i++) {
				for (int j = 0; j < numLines; j++) {
					file >> limits[i][j];
				}
			}

			// length of line
			for (int i = 0; i < numLines; i++) {
				file >> lengthLines[i];
			}

			// trip time
			for (int i = 0; i < numCars; i++) {
				file >> tripTime[i];
			}

			// type of schedule
			for (int i = 0; i < numCars; i++) {
				file >> schedule[i];
			}

			// skip a line
			getline(file, line);
			getline(file, line);

			// blocking list
			while (getline(file, line)) {
				Block block;
				//cout << "Line: " << line.c_str() << '\n';
				string part;
				istringstream strLine(line);

				// first read blocking line
				getline(strLine, part, ' ');
				block.blockingLine = atoi(part.c_str());

				while (getline(strLine, part, ' ')) {
					(block.blockedLines).push_back(atoi(part.c_str()));
				}
				blockList.push_back(block);
			}


			//printBlockList();

		}
	}


	int findMaxLength(){
		int max = 0;
		for (int i = 0; i < cars.size(); i++){
			if (max < cars[i].length) max = cars[i].length;
		}
		return max;
	}


	int findMinLength(){
		int min = 1000;
		for (int i = 0; i < cars.size(); i++){
			if (min > cars[i].length) min = cars[i].length;
		}
		return min;
	}

	bool strictLimits(Line line, Car car){
		//if ((line.length >= ((car.length * 2) + 1)) || (line.length >= ((car.length * 3) + 1))) return true;
		
		if (car.length == findMaxLength()){ // max length
			if (line.length < (findMinLength()*2) || (line.length < (findMinLength()*3+1)  && line.length >= (car.length*2+1) )) return true;
			else return false;
		}
		else if (car.length == findMinLength()){ // min length
			if ((line.length >= ((car.length * 2) + 1)) || (line.length >= ((car.length * 3) + 1))) return true;
			else return false;
		} else { // in the middle
			if (line.length >= (car.length*2 + 1) && line.length < (findMinLength()*3+1)) return true;
			else return false;
		}

	}

	int myrandom(int i) { return std::rand() % i; }

	/*
	* STARTING INSTANCE - sorts cars by trip time ascending
	*/
	void startingInstance() {
		// create vector of cars
		cars = createCars(numCars, lengthCars, typeCars, tripTime, schedule, numLines, limits);
		//printCars(cars);
		lines = createLines(numLines, lengthLines, cars);
		// sort cars by trip time ascending
		cars = sortCarsByTime(cars);

		vector<Block> blockListCopy = blockList;


			//***************** TRY 2.0 **************************************//
				// Try for greedy optimization - order is not random!
				// First fill blocking lines with least cars, then normal lines with least cars, then other blocking lines, then other normal lines

		vector<int> ordered; // final order for filling, as described
		vector<int> nonBB; // normal lines, without all blocking and blocked

		for (int i = 1; i <= numLines; i++) {
			nonBB.push_back(i);
		}

		// remove blocking and blocked
		for (int i = 0; i < blockList.size(); i++) {
			for (int j = 0; j < blockList[i].blockedLines.size(); j++) {
				nonBB.erase(remove(nonBB.begin(), nonBB.end(), blockList[i].blockedLines[j]), nonBB.end());
			}
			nonBB.erase(remove(nonBB.begin(), nonBB.end(), blockList[i].blockingLine), nonBB.end());
		}
		vector<int> blockInd;
		for (int i = 0; i < blockList.size(); i++) {
			blockInd.push_back(blockList[i].blockingLine);
		}
		vector<LineAllow> laB; // Blocking lines
		laB = fillLineAllow(blockInd); // blocking lines and nums
		laB = sortLineAllow(laB);

		vector<LineAllow> laN; // other lines
		laN = fillLineAllow(nonBB);
		laN = sortLineAllow(laN);

		// first blocking with least cars
		for (int i = 0; i < laB.size(); i++) {
			if (laB[i].allow < numCars) {
				ordered.push_back(laB[i].index);
			}
		}

		// then normal with least cars
		for (int i = 0; i < laN.size(); i++) {
			if (laN[i].allow < numCars) {
				ordered.push_back(laN[i].index);
			}
		}

		// blocking other
		for (int i = 0; i < laB.size(); i++) {
			if (laB[i].allow == numCars) {
				ordered.push_back(laB[i].index);
			}
		}

		// normal other
		for (int i = 0; i < laN.size(); i++) {
			if (laN[i].allow == numCars) {
				ordered.push_back(laN[i].index);
			}
		}



//*********************** TRY 2 - LAME GREEDY OPTIMIZATION **********************//

	// fill lines
	Line line;
	int index = 0;
	int done = 0;
	vector<Car> unsetCars, carsFill, carsTmp;
	vector<int> todo; // used for second try!!!!! ordered 

	for (int i = 0; i < cars.size(); i++){
		bool set = false;
		Car car = cars[i];


		todo = ordered;

		
		// try to put
		while (todo.size() > 0){
			index = todo[0];
			line = lines[index-1];
			index = todo[0];
			line = lines[index-1];
			if ((isBlocking(blockList, line) && strictLimits(line, car)) && canAddToLine(line, car)){
				line = addCarToLine(line, car);
				lines.at(index-1) = line;
				done++;
				set = true;
				break;
			}
			else {
				todo.erase(todo.begin());
			}
		}
		


		if (!set){ // try one more time without all limits
			todo = ordered;
			// try to put
			while (todo.size() > 0){
				index = todo[0];
				line = lines[index-1];
		
				
				if (strictLimits(line, car) && canAddToLine(line, car)){
					line = addCarToLine(line, car);
					lines.at(index-1) = line;
					done++;
					set = true;
					break;
				}
				else {
					todo.erase(todo.begin());
				}	
			}
		}

		if (!set){ // try one more time without all limits
			todo = ordered;
			// try to put
			while (todo.size() > 0){
				index = todo[0];
				line = lines[index-1];
				
				if (canAddToLine(line, car)){
					line = addCarToLine(line, car);
					lines.at(index-1) = line;
					done++;
					set = true;
					break;
				}
				else {
					todo.erase(todo.begin());

				}
				
			}
		}

		if (set){
			// check if some blocking line is full
			Line bLine;
			for (int i = 0; i < blockListCopy.size(); i++){
				bLine = getLine(lines, blockListCopy[i].blockingLine);
				double left = bLine.length - usedLength(bLine) + 0.5;
				bool full = false;
				if (line.type == 1 && left < 53) full = true;
				else if (line.type == 2 && left < 42) full = true;
				else if (line.type == 3 && left < 44) full = true;
				else full = false;
				if (full){ // full line
					// remove blocking line
					ordered.erase(remove(ordered.begin(), ordered.end(), blockListCopy[i].blockingLine), ordered.end());
					// add blocked lines

					vector<Block> tmpBlock;
					tmpBlock = blockListCopy; // block list without this blocking line, which will be removed
					tmpBlock.erase(tmpBlock.begin() + i);
					for (int j = 0; j < blockListCopy[i].blockedLines.size(); j++){
						if (!stillBlocked(tmpBlock, blockListCopy[i].blockedLines[j])) // add if its not blocked by some other blocking line
							//ordered.push_back(blockListCopy[i].blockedLines[j]);
							ordered.insert(ordered.begin(), blockListCopy[i].blockedLines[j]);
					}
					blockListCopy.erase(blockListCopy.begin() + i);
				}
			}
		}
		else unsetCars.push_back(car);
	}

// ******************** end *****************************//

	// ********** print lines ***************//
		//cout << "\n";
		//printCars(unsetCars);
		//cout << "\n";

		printLinesAll(lines);
		//cout << "\n";
		//cout << done << "\n";

		//cout << "lines 1: " << linesWithType(lines, 1) << " 2: " << linesWithType(lines, 2) << "\n";



		//printLinesGoodFormat(lines);
	}


	



	int main(int argc, char** argv) {
		if (argc == 2) {
			readFromFile(argv);
		}
		std::string number = argv[1];
		auto charn=number[8];
		startingInstance();
		goal::init_carlength(cars);
		int population_size = 10;
		int elitism = 1;
		double mutation_probability = 0.4;
		bool first = false;
		bool second = false;
		bool third = false;
		auto ga = GeneticAlgorithm(elitism, population_size, mutation_probability);
		auto c = Chromosom();
		c.chromosom_representation = lines;
		c.blockList = blockList;
		c.evaluation();
		ga.initialize(c);
		int cunt = 0;
		auto start = std::chrono::high_resolution_clock::now();
		bool condition = false;
		while (true) {
			ga.next_gen();
			//cout << cunt<<"\n";
			//cout << std::chrono::duration_cast<std::chrono::minutes>(start - std::chrono::high_resolution_clock::now()).count() << "\n";
			cunt++;
			if (std::chrono::duration_cast<std::chrono::minutes>(start - std::chrono::high_resolution_clock::now()).count() <= -1 && first == false) {

				first = true;
				auto best = ga.evaluate();
				cout << "1min\n";

				std::string title = "res-1m-i";
				title += charn;
				//printLinesGoodFormat(best.chromosom_representation);
				write(title,best.chromosom_representation);
				/*cout << goal::firstGoal(best.chromosom_representation)<<"\n";
				cout << goal::firstSubgoalF(best.chromosom_representation)<<"\n";
				cout << goal::secondSubgoalF(best.chromosom_representation) << "\n";
				cout << goal::thirdSubgoalF(best.chromosom_representation) << "\n";
				cout << goal::secondGoal(best.chromosom_representation) << "\n";
				cout << goal::firstSubgoalG(best.chromosom_representation) << "\n";
				cout << goal::secondSubgoalG(best.chromosom_representation) << "\n";
				cout << goal::thirdSubgoalG(best.chromosom_representation) << "\n";*/
				//return 0;
				///write best to file res-1m-instancenumber
			}
			else if (std::chrono::duration_cast<std::chrono::minutes>(start - std::chrono::high_resolution_clock::now()).count() <= -5 && second == false) {

				second = true;
				auto best = ga.evaluate();
				cout << "5min\n";
				std::string title = "res-5m-i";
				title += charn;
				write(title, best.chromosom_representation);
				//printLinesGoodFormat(best.chromosom_representation);
				return 0;
				///write best to file res-5m-instancenumber
			}
			else if (third == false && condition) {
				third = true;
				auto best = ga.evaluate();

				printLinesGoodFormat(best.chromosom_representation);
				///write best to file res-n-instancenumber
				break;

			}


		}
		return 0;
	}

#endif // !1