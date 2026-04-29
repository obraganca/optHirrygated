#ifndef CONSTRUCTIVE_HEURISTIC_FORWARD_B_HPP
#define CONSTRUCTIVE_HEURISTIC_FORWARD_B_HPP

#include "AbstractConstructiveHeuristic.hpp"

namespace opthirrygated {

    class ConstructiveHeuristicForwardB : public AbstractConstructiveHeuristic {
    public:
        explicit ConstructiveHeuristicForwardB(Instance &instance)
                : AbstractConstructiveHeuristic(instance) {}

        Solution execute() override;
    };

} // namespace opthirrygated

#endif // CONSTRUCTIVE_HEURISTIC_FORWARD_B_HPP
