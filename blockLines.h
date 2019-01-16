#ifndef BL_H
#define BL_H

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


struct Block {
	int blockingLine;
	vector<int> blockedLines;
};

bool stillBlocked(vector<Block> blockl, int index) {
	for (int i = 0; i < blockl.size(); i++) {
		for (int j = 0; j < blockl[i].blockedLines.size(); j++) {
			if (index == blockl[i].blockedLines[j]) return true;
		}
	}
	return false;
}

int findBlockingLine(vector<Block> blocks, Line line){
	for (int i = 0; i < blocks.size(); i++) {
		for (int j = 0; j < blocks[i].blockedLines.size(); j++) {
			if (line.index == blocks[i].blockedLines[j]) return blocks[i].blockingLine;
		}
	}
	return 0;
}

bool canCarFit(vector<Block> blocks, vector<Line> lines, Line line, Car car){
	int idBlocking = findBlockingLine(blocks, line);
	if (idBlocking != 0){
		Line bLine = getLine(lines, idBlocking);
		if (bLine.cars[bLine.cars.size()-1].time < car.time)
			return true;
		else return false;
	}	
	else return true;
}


#endif