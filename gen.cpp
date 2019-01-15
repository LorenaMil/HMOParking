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
using namespace std;

int numCars = 0;
int numLines = 0;
int lengthCars[100];
int typeCars[100];
int limits[100][100];
int lengthLines[100];
int tripTime[100];
int schedule[100];



	struct Block {
		int blockingLine;
		vector<int> blockedLines;
	};
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

	void printBlockList() {
		for (int i = 0; i < blockList.size(); i++) {
			cout << blockList[i].blockingLine << ' ';
			for (int j = 0; j < blockList[i].blockedLines.size(); j++) {
				cout << blockList[i].blockedLines[j] << ' ';
			}
			cout << '\n';
		}
	}

	bool stillBlocked(vector<Block> blockl, int index) {
		for (int i = 0; i < blockl.size(); i++) {
			for (int j = 0; j < blockl[i].blockedLines.size(); j++) {
				if (index == blockl[i].blockedLines[j]) return true;
			}
		}
		return false;
	}

	void readFromFile() {
		ifstream file("instanca1.txt");
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

		//********************** TRY 1.0 ***************************//
			// Completely shuffled order, start filling blocked lines when blocking line is full

			// put non blocked lines, and blocking lines
			// after bocking lines is full, put blocked lines

		vector<int> indLines;
		for (int i = 1; i <= numLines; i++) {
			indLines.push_back(i);
		}

		//remove blocked
		for (int i = 0; i < blockList.size(); i++) {
			for (int j = 0; j < blockList[i].blockedLines.size(); j++) {
				indLines.erase(remove(indLines.begin(), indLines.end(), blockList[i].blockedLines[j]), indLines.end());
			}
		}
		//	random_shuffle ( indLines.begin(), indLines.end() );
		/*	for (int i = 0; i < indLines.size(); i++){
				cout << indLines[i] << " ";
			}*/
			/*
				vector<int> blockingLines;
				for (int i = 0; i < blockList.size(); i++){
					blockingLines.push_back(blockList[i].blockingLine);
				}
			*/

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


		//********** FIRST TRY -- RANDOM FILLING ***********************//
			// fill lines
		/*Line line;
		int index = 0;
		int done = 0;
		vector<Car> unsetCars;
		vector<int> shuffled; // used for first try!!!! shuffle indLines every time

		for (int i = 0; i < cars.size(); i++) {
			bool set = false;
			Car car = cars[i];

			shuffled = indLines;
			random_shuffle(shuffled.begin(), shuffled.end(), myrandom);





			// try to put
			while (shuffled.size() > 0) {

				//random_shuffle ( shuffled.begin(), shuffled.end(), myrandom );
				index = shuffled[0];
				line = lines[index - 1];
				if ((((car.length > 50 && line.length < 85) || (usedLength(line) > 40))
					|| (line.allowedCars < numCars - 1 && car.allowedLines.size() < numLines - 1))
					&& canAddToLine(line, car)) {

					line = addCarToLine(line, car);
					lines.at(index - 1) = line;
					done++;
					set = true;
					break;
				}
				else {
					shuffled.erase(shuffled.begin());
				}
			}

			if (!set) {
				shuffled = indLines;
				random_shuffle(shuffled.begin(), shuffled.end(), myrandom);
				while (shuffled.size() > 0) {
					//random_shuffle ( shuffled.begin(), shuffled.end(), myrandom );
					index = shuffled[0];
					line = lines[index - 1];
					if (canAddToLine(line, car)) {
						line = addCarToLine(line, car);
						lines.at(index - 1) = line;
						done++;
						set = true;
						break;
					}
					else {
						shuffled.erase(shuffled.begin());
					}
				}
			}


			if (set) {
				// check if some blocking line is full
				Line bLine;
				for (int i = 0; i < blockListCopy.size(); i++) {
					bLine = getLine(lines, blockListCopy[i].blockingLine);
					double left = bLine.length - usedLength(bLine) + 0.5;
					bool full = false;
					if (line.type == 1 && left < 53) full = true;
					else if (line.type == 2 && left < 42) full = true;
					else if (line.type == 3 && left < 44) full = true;
					else full = false;
					if (full) { // full line
						// remove blocking line
						indLines.erase(remove(indLines.begin(), indLines.end(), blockListCopy[i].blockingLine), indLines.end());
						// add blocked lines
						vector<Block> tmpBlock;
						tmpBlock = blockListCopy; // block list without this blocking line, which will be removed
						tmpBlock.erase(tmpBlock.begin() + i);
						for (int j = 0; j < blockListCopy[i].blockedLines.size(); j++) {
							if (!stillBlocked(tmpBlock, blockListCopy[i].blockedLines[j])) // add if its not blocked by some other blocking line
								indLines.push_back(blockListCopy[i].blockedLines[j]);
						}
						blockListCopy.erase(blockListCopy.begin() + i);
					}
				}
			}
			else unsetCars.push_back(car);
		}
		*/

		//*************** end ****************//


	//*********************** TRY 2 - LAME GREEDY OPTIMIZATION **********************//
	
		// fill lines
		Line line;
		int index = 0;
		int done = 0;
		vector<Car> unsetCars;
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
				if ((((car.length > 50 && line.length < 90) || (car.length < 50 && line.length > 84) || (usedLength(line) > 40))
					|| (line.allowedCars < numCars-1 && car.allowedLines.size() < numLines-1))
					&& ((line.cars.size() == 0 && linesWithType(lines, car.type) < 13) || line.cars.size() > 0)
					&& canAddToLine(line, car)){
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
					//random_shuffle ( shuffled.begin(), shuffled.end(), myrandom );

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
								ordered.push_back(blockListCopy[i].blockedLines[j]);
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

		//printLinesAll(lines);
		//cout << "\n";
		//cout << done << "\n";

		//cout << "lines 1: " << linesWithType(lines, 1) << " 2: " << linesWithType(lines, 2) << "\n";



		//printLinesGoodFormat(lines);
	}


	



	int main() {
		readFromFile();
		startingInstance();
		goal::init_carlength(cars);
		int population_size = 5;
		int elitism = 1;
		double mutation_probability = 0.1;
		bool first = false;
		bool second = false;
		bool third = false;
		auto ga = GeneticAlgorithm(elitism, population_size, mutation_probability);
		auto c = Chromosom();
		c.chromosom_representation = lines;
		c.evaluation();
		ga.initialize(c);
		int cunt = 0;
		auto start = std::chrono::high_resolution_clock::now();
		bool condition = false;
		while (true) {
			ga.next_gen();
			cout << cunt<<"\n";
			//cout << std::chrono::duration_cast<std::chrono::minutes>(start - std::chrono::high_resolution_clock::now()).count() << "\n";
			cunt++;
			if (std::chrono::duration_cast<std::chrono::minutes>(start - std::chrono::high_resolution_clock::now()).count() <= -1 && first == false) {

				first = true;
				auto best = ga.evaluate();
				cout << "1min\n";
				printLinesGoodFormat(best.chromosom_representation);
				return 0;
				///write best to file res-1m-instancenumber
			}
			else if (std::chrono::duration_cast<std::chrono::minutes>(start - std::chrono::high_resolution_clock::now()).count() <= -5 && second == false) {

				second = true;
				auto best = ga.evaluate();
				cout << "5min\n";
				printLinesGoodFormat(best.chromosom_representation);
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