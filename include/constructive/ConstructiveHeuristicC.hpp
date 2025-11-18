#ifndef CONSTRUCTIVE_HEURISTIC_C_HPP
#define CONSTRUCTIVE_HEURISTIC_C_HPP

#include "AbstractConstructiveHeuristic.hpp"

namespace opthirrygated {

    class ConstructiveHeuristicC : public AbstractConstructiveHeuristic {
    public:
        explicit ConstructiveHeuristicC(Instance &instance)
                : AbstractConstructiveHeuristic(instance) {}

        Solution execute() override;
    };

} // namespace opthirrygated

#endif // CONSTRUCTIVE_HEURISTIC_C_HPP
