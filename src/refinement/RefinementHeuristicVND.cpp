#include "refinement/RefinementHeuristicVND.hpp"
#include "../include/Instance.hpp"
#include "../include/Solution.hpp"
#include "interface/ILocalSearch.hpp"
#include "Measurer.hpp"
#include <float.h>

using namespace std;
using namespace opthirrygated;


Solution RefinementHeuristicVND::execute(Solution &solution,  std::vector<std::unique_ptr<opthirrygated::INeighborhood>> neighborhoods, ILocalSearch& localSearch) {
    if (solution.getSolution().size() == 0) return solution;
    Measurer measurer(inst);

    int k =0;

    while (k < neighborhoods.size()) {

        Solution aux = localSearch.run(solution, inst, *neighborhoods[k]);

        if (measurer.evaluate(aux) < measurer.evaluate(solution)) {
            solution = aux;
            k = 0; // reinicia quando há melhora
        } else {
            k++; // só incrementa quando não melhora
        }
    }
    return solution;
}
