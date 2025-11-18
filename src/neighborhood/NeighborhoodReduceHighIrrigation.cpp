#include "neighborhood/NeighborhoodReduceHighIrrigation.hpp"
#include <random>
using namespace opthirrygated;

#include "Measurer.hpp"

Solution NeighborhoodReduceHighIrrigation::execute(const Solution& s, Instance& inst) {
    Solution cand = s;
    std::vector<int> highIrrigationDays = cand.getHighIrrigationDays();
    if (highIrrigationDays.empty()) return cand;

    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dayDist(0, highIrrigationDays.size() - 1);
    int day = highIrrigationDays[dayDist(rng)];

    std::vector<int> lowLevels = {0, 1, 2, 10};
    std::uniform_int_distribution<int> levelDist(0, lowLevels.size() - 1);

    Measurer measurer(inst);
    int tries = 0;
    const int maxTries = 10;

    while (tries < maxTries) {
        int newLevel = lowLevels[levelDist(rng)];

        cand = s;
        cand.updateSolution(day, newLevel);
        cand.propagate(inst, day);

        if (measurer.isFeasible(cand, 0)) {
            return cand;
        }

        ++tries;
    }

    return s;
}


