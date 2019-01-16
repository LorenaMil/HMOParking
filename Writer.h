#include <iostream>
#include <vector>
#include <fstream>
#include "line.h"

void write(std::string path, const std::vector<Line> & output) {
	std::ofstream file(path+".txt");

	if (file.is_open()) {
		std::cout << "help";
		for (auto & line : output) {

			for (int j = 0; j < line.cars.size(); j++) {
				file << line.cars[j].index << " ";
			}
			file << "\n";

		}
	}

	file.close();

}

