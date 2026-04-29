#ifndef CONSTRUCTIVE_HEURISTIC_FORWARD_C_HPP
#define CONSTRUCTIVE_HEURISTIC_FORWARD_C_HPP

#include "AbstractConstructiveHeuristic.hpp"

namespace opthirrygated {

    class ConstructiveHeuristicForwardC : public AbstractConstructiveHeuristic {
    public:
        explicit ConstructiveHeuristicForwardC(Instance &instance)
                : AbstractConstructiveHeuristic(instance) {}

        Solution execute() override;
    };

} // namespace opthirrygated

#endif // CONSTRUCTIVE_HEURISTIC_FORWARD_C_HPP
