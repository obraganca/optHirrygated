#ifndef REFINEMENT_HEURISTIC_RVND_HPP
#define REFINEMENT_HEURISTIC_RVND_HPP

#include "refinement/AbstractRefinementHeuristic.hpp"
#include "interface/INeighborhood.hpp"
#include "interface/ILocalSearch.hpp"
#include <vector>
#include <memory>

namespace opthirrygated {

class RefinementHeuristicRVND : public AbstractRefinementHeuristic {
public:
    explicit RefinementHeuristicRVND(Instance& inst)
        : AbstractRefinementHeuristic(inst) {}

    Solution execute(Solution& solution,
                     std::vector<std::shared_ptr<INeighborhood>>& neighborhoods,
                     ILocalSearch& localSearch) override;

};

} // namespace opthirrygated
#endif // REFINEMENT_HEURISTIC_RVND_HPP