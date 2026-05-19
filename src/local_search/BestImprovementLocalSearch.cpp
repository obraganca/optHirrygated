#include "local_search/BestImprovementLocalSearch.hpp"
#include "Measurer.hpp"
using namespace opthirrygated;

Solution BestImprovementLocalSearch::run(Solution& s, Instance& inst, INeighborhood& nh) {
    Solution best = s;

    for (int i = 0; i < s.getSolution().size(); ++i) {
        Solution cand = nh.execute(best, inst); // chama o execute da vizinhança

        if (cand.getScore() - best.getScore()<0) { // minimização
            cand.constructCriticalLimitDelt(inst);
            return cand;
        }
    }
    return best;
}


