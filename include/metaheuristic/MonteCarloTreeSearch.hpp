#ifndef MONTE_CARLO_TREE_SEARCH_HPP
#define MONTE_CARLO_TREE_SEARCH_HPP

#include "AbstractMetaheuristic.hpp"
#include "interface/INeighborhood.hpp"

#include <memory>
#include <random>
#include <algorithm>

namespace opthirrygated {

    class MonteCarloTreeSearch : public AbstractMetaheuristic {

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

        vector<shared_ptr<INeighborhood>>neighborhoods;

        double mctsC;
        int mctsIters;
        int mctsRolloutDepth;

        mt19937_64 rng;


        void backup(MCTSNode* node, double reward);
        MCTSNode* treePolicy(MCTSNode* node);
        MCTSNode* expand(MCTSNode* node);
        MCTSNode* bestChild(MCTSNode* node, double c);
        double defaultPolicy(const Solution& sol,
                             unordered_map<int, int>& irrigationFreq,
                             unordered_map<std::string, double>& patternRewards);

        // Helper methods
        bool isValidCandidate(const Solution& candidate, const Solution& original);
        double calculatePatternBonus(const Solution& sol);


        int childrenSize=10;
    public:

        MonteCarloTreeSearch(Instance &instance,
                Measurer& measurer,
                vector<shared_ptr<INeighborhood>>&nh,
                double C = 1.2,
                int iter = 50,
                int rollout_d = 10
        ): AbstractMetaheuristic(instance, measurer), mctsC(C), mctsIters(iter), mctsRolloutDepth(rollout_d), neighborhoods(nh){};

        Solution execute(Solution& solution) override;

        Solution executeSolo(opthirrygated::Solution &rootSolution);

        const std::vector<std::shared_ptr<INeighborhood>>& getNeighborhoods() const {
            return neighborhoods;
        }
    };

}

#endif // MONTE_CARLO_TREE_SEARCH_HPP
