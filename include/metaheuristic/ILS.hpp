#ifndef OPTHIRRYGATED_ILS_HPP
#define OPTHIRRYGATED_ILS_HPP

#include "metaheuristic/AbstractMetaheuristic.hpp"
#include "interface/IRefinementHeuristic.hpp"
#include "interface/INeighborhood.hpp"
#include "interface/ILocalSearch.hpp"
#include "../include/Instance.hpp"
#include "../include/Solution.hpp"
#include "Measurer.hpp"

#include <vector>
#include <memory>
#include <random>

namespace opthirrygated {

class ILS : public AbstractMetaheuristic {

    int maxIterations;
    int perturbStrength;

    std::vector<std::shared_ptr<INeighborhood>>& neighborhoods;
    ILocalSearch&                                localSearch;
    IRefinementHeuristic&                      refinement;

    mt19937 rng;


    Solution perturb(const Solution& s);

public:

    ILS(Instance& inst,
         Measurer& measurer,
         std::vector<std::shared_ptr<INeighborhood>>& neighborhoods,
         ILocalSearch& localSearch,
         IRefinementHeuristic& refinement,
         int maxIterations,
         int perturbStrength)
    : AbstractMetaheuristic(inst, measurer),
      maxIterations(maxIterations),
      perturbStrength(perturbStrength),
      neighborhoods(neighborhoods),
      localSearch(localSearch),
      refinement(refinement),
      rng(std::random_device{}()) {}

    Solution execute(Solution& solution) override;
};

} // namespace opthirrygated

#endif // OPTHIRRYGATED_ILS_HPP