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
        std::uniform_int_distribution<int> dayDist(0, std::max(0, D -1));
        int startDay = dayDist(rng);

        std::vector<int> lowLevels = {0, 1, 2, 10};
        int level = lowLevels[ std::uniform_int_distribution<int>(0, lowLevels.size()-1)(rng) ];

        int seqLength = std::uniform_int_distribution<int>(3, 5)(rng);
        int endDay = std::min(startDay + seqLength, D);

        float deltaScore = 0.0f;

        float lampDiff = 0.0f;
        bool ok = true;
        for (int i = startDay; i < endDay; ++i) {
            cand.updateSolution(i, level);

            lampDiff += inst.getLamp()[s.getSolution()[i]] - inst.getLamp()[level];

            deltaScore += (inst.getCost()[level] - inst.getCost()[s.getSolution()[i]]);

            if (!measurer.isFeasible(s, i, lampDiff)) {
                ok = false;
                break;
            }
        }

        if (!ok) { attempts++; continue; }

        // Agora sim, atualizar tudo
        cand.propagate(inst, startDay);
        cand.constructCriticalLimitDelt(inst);

        // Score deve ser s.score + delta
        cand.setScore( s.getScore() + deltaScore );

        return cand;
    }

    return s;
}

