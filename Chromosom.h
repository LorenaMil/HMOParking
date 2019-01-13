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
			auto line_index = rand() % chromosom_representation.size();
			auto line = chromosom_representation[line_index];
			///select random car
			auto index1 = rand() % line.cars.size();
			auto car = line.cars[index1];

			///select line of same type but not the same line
			auto line_index2 = rand()&chromosom_representation.size();
			auto line2=chromosom_representation[line_index2];
			while (line2.type != line.type) {
				line_index2 = rand()&chromosom_representation.size();
				line2 = chromosom_representation[line_index2];
			}
			///select car from line 2
			auto index2 = rand() % line2.cars.size();
			auto car2 = line2.cars[rand() % line2.cars.size()];

			///check cond 8
			auto block = block_list();
			bool cond8 = false;
			///todo check cond 8
			for (auto blocking : block) {
				auto blocked_lines = blocking.blockedLines;
				auto line1_in_blocked_lines = std::find(blocked_lines.begin(), blocked_lines.end(), line_index) != blocked_lines.end();
				auto line2_in_blocked_lines = std::find(blocked_lines.begin(), blocked_lines.end(), line_index2) != blocked_lines.end();
				///line 1 not in blocked lines
				if (!line1_in_blocked_lines) 
				{
					///line 2 not in blocked lines
					if (line2_in_blocked_lines) {
						cond8 = true;
						continue;
					}
				}
				else {
					auto blocking_line = blocking.blockingLine;
					auto bl = chromosom_representation[blocking_line];

					///last car in blocking line gets out before first car in line 1
					bool timecheck1 = bl.cars[bl.cars.size()].time < chromosom_representation[line_index].cars[0].time;
					

				}
			}
			///swap them
			line.cars[index1] = car2;
			line2.cars[index2] = car;


			///sort cars
			line.cars = sortCarsByTime(line.cars);
			line2.cars = sortCarsByTime(line2.cars);
			evaluation();
		}
		static Chromosom GetChild(const Chromosom &, const Chromosom &);
		static Chromosom Random();
		
		double evaluation() {
			fitness = secondGoal(chromosom_representation) / (firstGoal(chromosom_representation) + .0001);
			return fitness;
		}

		bool operator<(const Chromosom & rhs) const { return fitness < rhs.fitness; }

		bool operator==(const Chromosom & rhs) const { return fitness == rhs.fitness; }





};
#endif // !1