class Chromosom {


	public:
		double fitness;
		void mutate();
		static Chromosom GetChild(const Chromosom &, const Chromosom &);
		static Chromosom Random();
		double evaluation();

		bool operator<(const Chromosom & rhs) const { return fitness < rhs.fitness; }

};
