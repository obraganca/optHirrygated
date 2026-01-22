#ifndef CONSTRUCTIVE_HEURISTIC_BACKWARD_HPP
#define CONSTRUCTIVE_HEURISTIC_BACKWARD_HPP

#include "AbstractConstructiveHeuristic.hpp"

namespace opthirrygated {

    class ConstructiveHeuristicBackward : public AbstractConstructiveHeuristic {
        void normalizerAdf(vector<int> &solution, vector<float> &solutionAdf, float &score);
    public:
        explicit ConstructiveHeuristicBackward(Instance &instance)
                : AbstractConstructiveHeuristic(instance) {}

        Solution execute() override;
    };

} // namespace opthirrygated

#endif // CONSTRUCTIVE_HEURISTIC_BACKWARD_HPP
