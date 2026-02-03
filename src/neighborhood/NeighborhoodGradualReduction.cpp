#include "interface/INeighborhood.hpp"
#include "neighborhood/NeighborhoodGradualReduction.hpp"
#include <random>
#include <algorithm>

using namespace opthirrygated;

Solution NeighborhoodGradualReduction::execute(const Solution& s, Instance& inst) {
    Measurer measurer(inst);
    if (s.getSolution().empty()) return s;
    if (inst.getLowLevels().empty()) return s;

    auto lowLevels = inst.getLowLevels();

    int D = s.getSolution().size();
    static std::mt19937 rng(std::random_device{}());

    std::uniform_int_distribution<int> dayDist(0, D - 1);
    int day = dayDist(rng);
    int current = s.getSolution()[day];

    if (inst.getLamp()[current] > inst.getLamp()[inst.getMaxLowLevel()]) {

        std::shuffle(lowLevels.begin(), lowLevels.end(), rng);

        for (int newLevel : lowLevels) {
            Solution cand = s;
            cand.updateSolution(day, newLevel);


            if (measurer.isFeasible(cand, day, inst.getLamp()[s.getSolution()[day]] - inst.getLamp()[newLevel])) {

                cand.propagate(inst, day);
                cand.constructCriticalLimitDelt( inst);
                cand.setScore(s.getScore() + (inst.getCost()[newLevel] - inst.getCost()[s.getSolution()[day]]));
                return cand;
            }
            break;
        }
    }
    return s;
}