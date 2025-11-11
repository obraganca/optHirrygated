#ifndef CONSTRUCTIVE_HEURISTIC_B_HPP
#define CONSTRUCTIVE_HEURISTIC_B_HPP

#include "AbstractConstructiveHeuristic.hpp"

namespace opthirrygated {

    class ConstructiveHeuristicB : public AbstractConstructiveHeuristic {
    public:
        explicit ConstructiveHeuristicB(Instance &instance)
                : AbstractConstructiveHeuristic(instance) {}

        Solution execute() override;
    };

} // namespace opthirrygated

#endif // CONSTRUCTIVE_HEURISTIC_B_HPP
