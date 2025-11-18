#ifndef REFINEMENT_HEURISTIC_VND_HPP
#define REFINEMENT_HEURISTIC_VND_HPP

#include "AbstractRefinementHeuristic.hpp"
#include "interface/INeighborhood.hpp"
#include "interface/ILocalSearch.hpp"
#include <vector>
#include <memory>


namespace opthirrygated {

    class RefinementHeuristicVND : public AbstractRefinementHeuristic {
    public:
        explicit RefinementHeuristicVND(Instance &instance)
                : AbstractRefinementHeuristic(instance) {}

        Solution execute(Solution &solution,  std::vector<std::unique_ptr<opthirrygated::INeighborhood>> neighborhoods, ILocalSearch& localSearch) override;
    };

} // namespace opthirrygated

#endif // CONSTRUCTIVE_HEURISTIC_VND_HPP
