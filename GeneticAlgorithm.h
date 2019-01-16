#include <vector>
#include <unordered_map>
#include <iostream>
#include <algorithm>
#include <random>
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
		

		void initialize(const Chromosom & c) {
			population.reserve(populationSize);
			population.push_back(c);
			while (population.size() < populationSize-1) {
				auto c2 = Chromosom::Random(c);
				c2.evaluation();
				population.push_back(c2);
			}
			set_fitness_sum();

		}

		Chromosom evaluate() {
		
			std::sort(population.begin(), population.end());
			return population[0];
		
		}

		void next_gen() {
			next_generation_elimination();
			set_fitness_sum();
		};


		GeneticAlgorithm(int elite, int popsize, double mutprob) :elitism(elite), populationSize(popsize), mutationProbability(mutprob) {};

		GeneticAlgorithm(int elite, int popsize, double mutprob,bool elimination) :elitism(elite), populationSize(popsize), mutationProbability(mutprob),elimination(elimination) {};
	
	private:
		std::vector<Chromosom> tournament;

		void next_gen_generational() {};

		void next_generation_elimination() {
			auto tournament = std::vector<Chromosom>();
			tournament.reserve(3);
			srand(time(0));
			std::vector<int> indices = { 0,0,0 };
			for (int i = 0;i < 3;i++) {
				indices[i] = rand() % population.size();
				tournament.push_back(population[indices[i]]);
			}
			
			auto toRemove = tournament[0];
			auto index = 0;
			//finding element to remove
			for (int i = 1;i < 3;i++) {
				if (tournament[i] < toRemove) {
					toRemove = tournament[i];
					index = i;
				}
			}
			tournament.erase(tournament.begin()+index);
			population.erase(population.begin()+indices[index]);
			auto parent1 = tournament[0];
			auto parent2 = tournament[1];
			auto child = Chromosom::GetChild(parent1,parent2);
			if (rand() % 100 < mutationProbability * 100) {
				child.mutate();
			}
			child.evaluation();
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