#ifndef CONSTRUCTIVE_HEURISTIC_LOOKAHEAD_HPP
#define CONSTRUCTIVE_HEURISTIC_LOOKAHEAD_HPP

#include "AbstractConstructiveHeuristic.hpp"

namespace opthirrygated {

    class ConstructiveHeuristicLookahead : public AbstractConstructiveHeuristic {
    private:
        int lookaheadDepth;
    public:
        explicit ConstructiveHeuristicLookahead(Instance &instance, int depth = 1)
                : AbstractConstructiveHeuristic(instance), lookaheadDepth(depth) {}

        Solution execute() override;

    private:
        float simulateLookahead(size_t day, float adi, int depth);
    };

}

#endif // CONSTRUCTIVE_HEURISTIC_LOOKAHEAD_HPP
