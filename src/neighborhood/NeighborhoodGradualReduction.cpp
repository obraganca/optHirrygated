#include "interface/INeighborhood.hpp"
#include "neighborhood/NeighborhoodGradualReduction.hpp"
#include <random>
#include <algorithm>

using namespace opthirrygated;

Solution NeighborhoodGradualReduction::execute(const Solution& s, Instance& inst) {
    Measurer measurer(inst);

    int D = s.getSolution().size();
    static std::mt19937 rng(std::random_device{}());

    std::uniform_int_distribution<int> dayDist(0, D - 1);
    int day = dayDist(rng);
    int current = s.getSolution()[day];

    bool feasible=false;
    if (current >= 3 && current <   10) {
        std::vector<int> reductionOptions = {0, 1, 2, 10};

        std::shuffle(reductionOptions.begin(), reductionOptions.end(), rng);

        for (int newLevel : reductionOptions) {
            Solution cand = s;
            cand.updateSolution(day, newLevel);


            if (measurer.isFeasible(cand, day, inst.getLamp()[s.getSolution()[day]] - inst.getLamp()[newLevel])) {

                cand.propagate(inst, day);
                cand.constructCriticalLimitDelt( inst);
                cand.setScore(s.getScore() + (inst.getCost()[newLevel] - inst.getCost()[s.getSolution()[day]]));
                return cand;
            }
        }
    }
    return s;
}