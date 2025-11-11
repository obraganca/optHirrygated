#ifndef CONSTRUCTIVE_HEURISTIC_A_HPP
#define CONSTRUCTIVE_HEURISTIC_A_HPP

#include "AbstractConstructiveHeuristic.hpp"

namespace opthirrygated {

    class ConstructiveHeuristicA : public AbstractConstructiveHeuristic {
    public:
        ConstructiveHeuristicA(Instance &instance)
                : AbstractConstructiveHeuristic(instance) {}

        Solution execute() override;
    };

} // namespace opthirrygated

#endif // CONSTRUCTIVE_HEURISTIC_A_HPP
