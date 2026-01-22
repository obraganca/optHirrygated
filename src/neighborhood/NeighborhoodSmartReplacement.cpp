#include "neighborhood/NeighborhoodSmartReplacement.hpp"
#include "Measurer.hpp"
#include <random>
#include <limits>

using namespace opthirrygated;

Solution NeighborhoodSmartReplacement::execute(const Solution& s, Instance& inst) {
    Measurer measurer(inst);
    Solution bestCand = s;
    std::vector<int> highIrrigationDays = bestCand.getHighIrrigationDays();
    if (highIrrigationDays.empty())
        return s;

    static std::mt19937 rng(std::random_device{}());
    std::uniform_int_distribution<int> dayDist(0, highIrrigationDays.size() - 1);
    int day = highIrrigationDays[dayDist(rng)];
    std::vector<int> lowLevels = {0, 1, 2, 10};

    for (int newLevel : lowLevels) {
        Solution cand = s;
        cand.updateSolution(day, newLevel);

        if (!measurer.isFeasible(cand, day, (float)(inst.getLamp()[s.getSolution()[day]] - inst.getLamp()[newLevel])))
            continue;

        cand.setScore(s.getScore() + (inst.getCost()[newLevel] - inst.getCost()[bestCand.getSolution()[day]]));

        if (cand.getScore() - bestCand.getScore() < 0) {
            bestCand = cand;
        }
    }

    bestCand.propagate(inst, day);
    bestCand.constructCriticalLimitDelt( inst);
    return bestCand;
}
