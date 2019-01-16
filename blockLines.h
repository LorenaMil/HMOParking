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

bool stillBlocked(const vector<Block> & blocks, int index) {
	for (int i = 0; i < blocks.size(); i++) {
		for (int j = 0; j < blocks[i].blockedLines.size(); j++) {
			if (index == blocks[i].blockedLines[j]) return true;
		}
	}
	return false;
}

bool isBlocking(const vector<Block> & blocks, Line line){
	for (int i = 0; i < blocks.size(); i++) {
		if (line.index == blocks[i].blockingLine) return true;
	}
	return false;
}

vector<int> findBlockingLine(const vector<Block> & blocks, Line line){
	vector<int> allBlocking;
	for (int i = 0; i < blocks.size(); i++) {
		for (int j = 0; j < blocks[i].blockedLines.size(); j++) {
			if (line.index == blocks[i].blockedLines[j]) allBlocking.push_back(blocks[i].blockingLine);
		}
	}
	return allBlocking;
}

// test if this is blocking line and one of blocked lines
bool blockPair(const vector<Block> & blocks, Line lineBlocking, Line lineBlocked){
	for (int i = 0; i < blocks.size(); i++) {
		if (lineBlocking.index == blocks[i].blockingLine){
			for (int j = 0; j < blocks[i].blockedLines.size(); j++) {
				if (lineBlocked.index == blocks[i].blockedLines[j]) return true;
			}
		}
		
	}
	return false;
}

vector<int> getBlockedLines(const vector<Block> & blocks, Line line){
	vector<int> allBlocked;
	for (int i = 0; i < blocks.size(); i++) {
		if (line.index == blocks[i].blockingLine){
			for (int j = 0; j < blocks[i].blockedLines.size(); j++) 
				allBlocked.push_back(blocks[i].blockedLines[j]);
		}
	}
	return allBlocked;
}

bool canCarFit(const vector<Block> & blocks, const vector<Line> & lines, Line line, Car car){
	vector<int> allBlocking = findBlockingLine(blocks, line);
	int count1 = 0, count2 = 0;
	if (allBlocking.size() != 0){ // if line is blocked
		for (int i = 0; i < allBlocking.size(); i++){
			Line bLine = getLine(lines, allBlocking[i]);
			if (bLine.cars.size() > 0 && bLine.cars[bLine.cars.size()-1].time < car.time)
				count1++;
		}
		if (count1 == allBlocking.size()) return true;
		else return false;
	}
	else if (isBlocking(blocks, line)){ // line is blocking, add only if car time is before first car in every blocked line
		vector<int> allBlocked = getBlockedLines(blocks, line);
		for (int i = 0; i < allBlocked.size(); i++){
			Line bLine2 = getLine(lines, allBlocked[i]);
			if (bLine2.cars.size() > 0 && car.time < bLine2.cars[0].time)
				count2++;
		}
		if (count2 == allBlocked.size()) return true;
		else return false;
	}	
	else return true; // line is normal, no limits
}


#endif