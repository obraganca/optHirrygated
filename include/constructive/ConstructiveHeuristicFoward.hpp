#ifndef CONSTRUCTIVE_HEURISTIC_FOWARD_HPP
#define CONSTRUCTIVE_HEURISTIC_FOWARD_HPP

#include "AbstractConstructiveHeuristic.hpp"

namespace opthirrygated {

    class ConstructiveHeuristicFoward : public AbstractConstructiveHeuristic {
    public:
        ConstructiveHeuristicFoward(Instance &instance)
                : AbstractConstructiveHeuristic(instance) {}

        Solution execute() override;
    };

} // namespace opthirrygated

#endif // CONSTRUCTIVE_HEURISTIC_Foward_HPP
