#ifndef CAR_H
#define CAR_H

#include <string>
#include <vector>
#include <stdlib.h>
#include <iostream>
#include <unordered_map>

using namespace std;

struct Car{
	int index;
	int length;
	int type;
	int time;
	int schedule;

	unordered_map<int,bool> allowedLines;

	bool operator < (const Car& car) const
    {
        return (time < car.time);
    }
};

vector<Car> createCars(int numcars, int length[], int tripTime[], int type[], int schedule[], int numlines, int limit[][100]);

vector<Car> sortCarsByTime(vector<Car> cars);

void printCars(vector<Car> cars);

bool allowedLineForCar(int indexLine, const Car & car);

int totalCarsLength(vector<Car> cars);

void printCarWithID(vector<Car> cars, int id);

int carsWithType(vector<Car> cars, int type);



#endif