#include "neighborhood/NeighborhoodPatternBased.hpp"
#include <random>

using namespace opthirrygated;

Solution NeighborhoodPatternBased::execute(const Solution& s, Instance& inst) {
    Solution cand = s;
    int D = cand.getSolution().size();
    static std::mt19937 rng(std::random_device{}());
    Measurer measurer(inst);

    int attempts = 0;
    const int max_attempts = 1000;

    while (attempts < max_attempts) {
        cand = s;
        std::uniform_int_distribution<int> dayDist(0, D - 5);
        int startDay = dayDist(rng);

        std::vector<int> lowLevels = {0, 1, 2, 10};
        std::uniform_int_distribution<int> levelDist(0, lowLevels.size() - 1);
        int level = lowLevels[levelDist(rng)];

        int seqLength = std::uniform_int_distribution<int>(3, 5)(rng);
        for (int i = 0; i < seqLength && (startDay + i) < D; ++i) {
            cand.updateSolution(startDay + i, level);
        }

        cand.propagate(inst, startDay);
        if (measurer.isFeasible(cand, startDay)) {
            return cand;
        }

        attempts++;
    }
    return s;
}