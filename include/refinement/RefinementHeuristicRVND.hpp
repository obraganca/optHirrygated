#ifndef OPTHIRRYGATED_REFINEMENTHEURISTICVND_HPP
#define OPTHIRRYGATED_REFINEMENTHEURISTICVND_HPP

#include "refinement/AbstractRefinementHeuristic.hpp"
#include "interface/ILocalSearch.hpp"
#include <memory>
#include <vector>

namespace opthirrygated {

    class RefinementHeuristicVND : public AbstractRefinementHeuristic {
    public:
        RefinementHeuristicVND(Instance& inst) : AbstractRefinementHeuristic(inst) {}

        virtual Solution execute(Solution& solution,
                                std::vector<std::shared_ptr<INeighborhood>>& neighborhoods,
                                ILocalSearch& localSearch) override;
    };

} // namespace opthirrygated

#endif // OPTHIRRYGATED_REFINEMENTHEURISTICVND_HPP