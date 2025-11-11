#ifndef MONTE_CARLO_TREE_SEARCH_HPP
#define MONTE_CARLO_TREE_SEARCH_HPP

#include "interface/IMetaheuristica.hpp"

#include <memory>
#include <random>

namespace opthirrygated {

    class MonteCarloTreeSearch : public IMetaheuristica {

        Instance& inst;

        struct MCTSNode {
            Solution sol;
            MCTSNode* parent;
            std::vector<std::unique_ptr<MCTSNode>> children;
            double W = 0.0;    // total reward (negative cost)
            int N = 0;         // visit count
            bool fullyExpanded = false;
            MCTSNode(const Solution& s, MCTSNode* p = nullptr)
                    : sol(s), parent(p) {}
        };


        double mctsC;
        int mctsIters;
        int mctsRolloutDepth;

        mt19937_64 rng;

    public:
        MonteCarloTreeSearch(Instance &instance,
                double C = 1.4142,
                int iter = 1000,
                int rollout_d = 20
        ): inst(instance), mctsC(C), mctsIters(iter), mctsRolloutDepth(rollout_d) {};
        Solution execute() override;
    };

}

#endif // MONTE_CARLO_TREE_SEARCH_HPP
