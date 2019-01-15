#include "car.h"
#include <string>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <algorithm>

using namespace std;

/*
struct Car{
	int index;
	int length;
	int type;
	int time;
	int schedule;

	vector<int> allowedLines;
};*/

vector<Car> createCars(int numcars, int length[], int type[], int tripTime[], int schedule[], int numlines, int limit[][100]){
	vector<Car> cars;
	for (int i = 0; i < numcars; i++){
		Car car;
		car.index = i+1;
		car.length = length[i];
		car.type = type[i];
		car.time = tripTime[i];
		car.schedule = schedule[i];

		for (int j = 0; j < numlines; j++){
			if (limit[i][j] == 1)
				car.allowedLines[j+1]=true;
			else {
				car.allowedLines[j + 1] = false;
			}
		}

		cars.push_back(car);
	}
	return cars;
}

vector<Car> sortCarsByTime(vector<Car> cars){
	sort(cars.begin(), cars.end());
	return cars;
}

void printCars(vector<Car> cars){
	cout << "Unset cars\n";
	for (unsigned i = 0; i < cars.size(); i++){
		cout << cars[i].index << " " << cars[i].type << " " << cars[i].time << " " << cars[i].length << "Lines: ";
		for(unsigned j = 0; j < cars[i].allowedLines.size(); j++){
			cout << cars[i].allowedLines[j] << " ";
		}
		cout << "\n";
	}
}

bool allowedLineForCar(int indexLine, const Car & car){
	if (car.allowedLines.count(indexLine)>0) {
		return car.allowedLines.at(indexLine);
	}
	return false;
}

int totalCarsLength(vector<Car> cars){
	int total = 0;
	for (int i = 0; i < cars.size(); i++){
		total += cars[i].length;
	}
	return total;
}

void printCarWithID(vector<Car> cars, int id){
	for (int i = 0; i < cars.size(); i++){
		if (cars[i].index == id){
			cout << "CAR: " << id << "\n";
			cout << "Length: " << cars[i].length << "\n";
			cout << "Type: " << cars[i].type << "\n";
			cout << "Time: " << cars[i].time << "\n";
			cout << "Schedule: " << cars[i].schedule << "\n\n";
		}
	}
}

int carsWithType(vector<Car> cars, int type){
	int count = 0;
	for (int i = 0; i < cars.size(); i++){
		if (cars[i].type == type) count++;
	}
	return count;
}