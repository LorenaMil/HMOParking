#ifndef goalf
#define goalf

#include "line.h"

namespace goal {
	int carl = 0;
	int carCount = 0;
	//********* FIRST GLOBAL GOAL **************//

		// First subgoal
	double firstSubgoalF(const vector<Line> & lines) {
		int factor = numUsedLines(lines)-1;
		int numDiff = numDiffTypes(lines);

		return (double)numDiff / factor;
	}

	// Second subgoal
	double secondSubgoalF(const vector<Line> & lines) {
		int factor = lines.size();
		int numL = numUsedLines(lines);

		return (double)numL / factor;
	}

	// Third subgoal
	double thirdSubgoalF(const vector<Line> & lines) {
		double cap = unusedCapacity(lines);
		int factor = totalLinesLength(lines) - carl;

		return (double)cap / factor;
	}

	double firstGoal(const vector<Line> & lines) {
		return (firstSubgoalF(lines) + secondSubgoalF(lines) + thirdSubgoalF(lines));
	}


	//************ SECOND GLOBAL GOAL *****************//

	// First subgoal
	double firstSubgoalG(const vector<Line> & lines) {
		int pairs = numSameScheduleInLine(lines);
		int factor = carCount - numUsedLines(lines);
		auto result = (double)pairs / factor;
		return result;
	}

	// Second subgoal
	double secondSubgoalG(const vector<Line> & lines) {
		int pairs = numSameScheduleLines(lines);
		int factor = numUsedLines(lines)-1;

		return ((double)pairs) / factor;
	}

	// Third subgoal
	double thirdSubgoalG(const vector<Line> & lines) {
		int time = timeDiff(lines);
		int factor = 15 * numOfNeighbours(lines);
		if (time == 0) { return 0; }
		return ((double)time) / factor;
	}

	double secondGoal(const vector<Line> & lines) {
		auto fsg = firstSubgoalG(lines);
		auto ssg = secondSubgoalG(lines);
		auto tsg = thirdSubgoalG(lines);
		return ( fsg+ ssg + tsg);
	}


	void init_carlength(const vector<Car>cars) {
		for (auto car : cars) {
			carl += car.length;
		}
		carCount = cars.size();
	}



}
#endif // !goalf