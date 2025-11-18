    #include "local_search/BestImprovementLocalSearch.hpp"
    #include "Measurer.hpp"
    using namespace opthirrygated;

    Solution BestImprovementLocalSearch::run(Solution& s, Instance& inst, INeighborhood& nh) {
        Solution best = s;
        Measurer measurer(inst);

        double bestCost = measurer.evaluate(s);

        for (int i = 0; i < s.getSolution().size(); ++i) {
            Solution cand = nh.execute(best, inst); // chama o execute da vizinhança
            double candCost = measurer.evaluate(cand);

            if (candCost < bestCost) { // minimização
                best = cand;
                bestCost = candCost;
            }
        }

        return best;
    }


