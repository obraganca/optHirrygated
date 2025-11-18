#ifndef I_ABSTRACT_REFINEMENT_HEURISTIC_HPP
#define I_ABSTRACT_REFINEMENT_HEURISTIC_HPP

#include "Instance.hpp"
#include "Solution.hpp"
#include "interface/IRefinementHeuristic.hpp"
#include "interface/INeighborhood.hpp"
#include "interface/ILocalSearch.hpp"

using namespace std;

namespace opthirrygated{

    class AbstractRefinementHeuristic : public IRefinementHeuristic{
    protected:
        Instance &inst;
    public:
        AbstractRefinementHeuristic(Instance &instance):inst(instance){}
        virtual Solution execute(Solution &solution,  std::vector<std::unique_ptr<opthirrygated::INeighborhood>> neighborhoods, ILocalSearch& localSearch) = 0;
    };
}

#endif