#include "GeneticAlgorithm.h"
#include "Chromosom.h"
#include <chrono>
int main() {



	int elitism = 1;
	int population_size = 10;
	double mutation_probability = 0.1;
	bool first=false;
	bool second=false;
	bool third=false;
	auto ga = GeneticAlgorithm(elitism,population_size,mutation_probability);
	ga.initialize();
	auto start = std::chrono::high_resolution_clock::now();
	bool condition = false;
	while (true) {
		ga.next_gen();
		if (std::chrono::duration_cast<std::chrono::minutes>(start - std::chrono::high_resolution_clock::now()).count > 1 && first == false) {

			first = true;
			auto best=ga.evaluate();
			///write best to file res-1m-instancenumber
		}
		else if (std::chrono::duration_cast<std::chrono::minutes>(start - std::chrono::high_resolution_clock::now()).count > 5 && second == false) {

			second = true;
			auto best = ga.evaluate();
			///write best to file res-5m-instancenumber
		}
		else if (third == false && condition) {
			third = true;
			auto best = ga.evaluate();
			///write best to file res-n-instancenumber
			break;

		}

	
	}

	return 0;
}