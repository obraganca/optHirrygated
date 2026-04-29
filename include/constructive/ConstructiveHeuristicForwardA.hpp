#ifndef CONSTRUCTIVE_HEURISTIC_FORWARD_A_HPP
#define CONSTRUCTIVE_HEURISTIC_FORWARD_A_HPP

#include "AbstractConstructiveHeuristic.hpp"

namespace opthirrygated {

    class ConstructiveHeuristicForwardA : public AbstractConstructiveHeuristic {
    public:
        ConstructiveHeuristicForwardA(Instance &instance)
                : AbstractConstructiveHeuristic(instance) {}

        Solution execute() override;
    };

} // namespace opthirrygated

#endif // CONSTRUCTIVE_HEURISTIC_FORWARD_B_HPP
