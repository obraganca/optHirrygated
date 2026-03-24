#include "refinement/RefinementHeuristicRVND.hpp"
#include <algorithm>
#include <random>

using namespace opthirrygated;

Solution RefinementHeuristicRVND::execute(Solution& solution,
                                          std::vector<std::shared_ptr<INeighborhood>>& neighborhoods,
                                          ILocalSearch& localSearch) {
    if (solution.getSolution().empty()) return solution;

    static std::mt19937 rng(std::random_device{}());

    // Cria índices para embaralhar sem alterar o vetor original
    std::vector<int> order(neighborhoods.size());
    std::iota(order.begin(), order.end(), 0);

    // Embaralha a ordem das vizinhanças (característica central do RVND)
    std::shuffle(order.begin(), order.end(), rng);

    int k = 0;
    while (k < (int)neighborhoods.size()) {
        Solution aux = localSearch.run(solution, inst, *neighborhoods[order[k]]);

        if (aux.getScore() < solution.getScore()) {
            solution = aux;
            // Reinicia com nova ordem aleatória ao encontrar melhoria
            std::shuffle(order.begin(), order.end(), rng);
            k = 0;
        } else {
            k++;
        }
    }

    return solution;
}