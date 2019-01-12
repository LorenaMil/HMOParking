#ifndef CHROMOSOM
#define CHROMOSOM
#include "car.h";
#include "line.h";
#include <vector>
#include "gen.cpp"
class Chromosom {


	public:
		vector<Line> chromosom_representation;
		double fitness;
		
		void mutate() {
			///select random line
			auto line = chromosom_representation[rand() % chromosom_representation.size()];
			///select random car
			auto index1 = rand() % line.cars.size();
			auto car = line.cars[index1];

			///select line of same type but not the same line
			auto line2=chromosom_representation[rand()&chromosom_representation.size()];
			while (line2.type != line.type) {
				line2 = chromosom_representation[rand()&chromosom_representation.size()];
			}
			///select car from line 2
			auto index2 = rand() % line2.cars.size();
			auto car2 = line2.cars[rand() % line2.cars.size()];

			///check cond 8
			auto block = block_list();
			///todo check cond 8

			///swap them
			line.cars[index1] = car2;
			line2.cars[index2] = car;


			///sort cars
			line.cars = sortCarsByTime(line.cars);
			line2.cars = sortCarsByTime(line2.cars);

		}
		static Chromosom GetChild(const Chromosom &, const Chromosom &);
		static Chromosom Random();
		
		double evaluation() {

			//return secondGoal(chromosom_representation)/(firstGoal(chromosom_representation)+.0001);
		}

		bool operator<(const Chromosom & rhs) const { return fitness < rhs.fitness; }

		bool operator==(const Chromosom & rhs) const { return fitness == rhs.fitness; }





};
#endif // !1