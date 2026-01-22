#ifndef I_REFINEMENT_HEURISTIC_HPP
#define I_REFINEMENT_HEURISTIC_HPP

#include <vector>
#include <memory>

#include "Solution.hpp"
#include "interface/INeighborhood.hpp"
#include "interface/ILocalSearch.hpp"

namespace opthirrygated{
    class IRefinementHeuristic{
    public:
        virtual ~IRefinementHeuristic()=default;
        virtual Solution execute(Solution &solution,
                        std::vector<std::shared_ptr<INeighborhood>>& neighborhoods,
                        ILocalSearch& localSearch) = 0;
    };
}

#endif