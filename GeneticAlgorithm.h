#include <vector>
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include "Chromosom.h"
#include <random>
#include <time.h>

class GeneticAlgorithm {
	
	public: 
		int elitism;
		int populationSize;
		double mutationProbability;
		bool elimination=true;
		std::vector<Chromosom> population;
		

		void initialize() {
		
			while (population.size() < populationSize) {
				population.push_back(Chromosom::Random());
			}

		}

		Chromosom evaluate() {
		
			std::sort(population.begin(), population.end());
			return population[0];
		
		}

		void next_gen() {
			if (elimination) next_generation_elimination();
			else next_gen_generational();
			set_fitness_sum();
		};


		GeneticAlgorithm(int elite, int popsize, double mutprob) :elitism(elite), populationSize(popsize), mutationProbability(mutprob) {};

		GeneticAlgorithm(int elite, int popsize, double mutprob,bool elimination) :elitism(elite), populationSize(popsize), mutationProbability(mutprob),elimination(elimination) {};
	
	private:


		void next_gen_generational();

		void next_generation_elimination() {
			auto tournament = std::vector<Chromosom>();
			tournament.reserve(3);
			srand(time(0));
			for (int i = 0;i < 3;i++) {
				tournament.push_back(population[rand()%populationSize]);
			}
			
			auto toRemove = tournament[0];
			//finding element to remove
			for (int i = 1;i < 3;i++) {
				if (tournament[i] < toRemove) {
					toRemove = tournament[i];
				}
			}
			tournament.erase(std::remove(tournament.begin(), tournament.end(), toRemove), tournament.end());
			population.erase(std::remove(population.begin(), population.end(), toRemove), population.end());
			auto parent1 = tournament[0];
			auto parent2 = tournament[1];
			auto child = Chromosom::GetChild(parent1,parent2);
			if (rand() % 100 < mutationProbability * 100) {
				child.mutate();
			}
			population.push_back(child);
		
		
		}

		double fitnessSum;

		void set_fitness_sum() {
			auto sum = 0.0;
			for (const auto &c : population) {
				sum+=c.fitness;
			}
			fitnessSum = sum;
		}
};