#ifndef CONSTRUCTIVE_HEURISTIC_LOOKAHEAD_MEMOIZATION_HPP
#define CONSTRUCTIVE_HEURISTIC_LOOKAHEAD_MEMOIZATION_HPP

#include "AbstractConstructiveHeuristic.hpp"
#include <unordered_map>

namespace opthirrygated {

    class ConstructiveHeuristicLookaheadMemoization : public AbstractConstructiveHeuristic {
    private:
        int lookaheadDepth;
    public:
        explicit ConstructiveHeuristicLookaheadMemoization(Instance &instance, int depth = 1)
                : AbstractConstructiveHeuristic(instance), lookaheadDepth(depth) {}

        Solution execute() override;

    private:

        unordered_map<long long, float> dp;
        float simulateLookahead(size_t day, float adi, int depth);
    };

}

#endif // CONSTRUCTIVE_HEURISTIC_LOOKAHEAD_MEMOIZATION_HPP
