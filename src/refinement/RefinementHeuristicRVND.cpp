#include "refinement/RefinementHeuristicVND.hpp"
#include "../include/Instance.hpp"
#include "../include/Solution.hpp"
#include "interface/ILocalSearch.hpp"
#include "Measurer.hpp"
#include <float.h>

using namespace std;
using namespace opthirrygated;

Solution RefinementHeuristicVND::execute(Solution& solution,
                                        std::vector<std::shared_ptr<INeighborhood>>& neighborhoods,
                                        ILocalSearch& localSearch) {
    if (solution.getSolution().size() == 0) return solution;


    int k = 0;
    while (k < neighborhoods.size()) {
        Solution aux = localSearch.run(solution, inst, *neighborhoods[k]);
        if (aux.getScore() < solution.getScore()) {
            solution = aux;
            k = 0; // restart when improvement found
        } else {
            k++; // move to next neighborhood
        }
    }
    return solution;
}