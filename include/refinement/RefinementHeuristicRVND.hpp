#ifndef REFINEMENT_HEURISTIC_RVND_HPP
#define REFINEMENT_HEURISTIC_RVND_HPP

#include "../include/Solution.hpp"
#include "../include/Instance.hpp"
#include "interface/INeighborhood.hpp"
#include "interface/ILocalSearch.hpp"
#include <vector>
#include <memory>

namespace opthirrygated {

    /**
     * RVND - Random Variable Neighborhood Descent
     *
     * Variação do VND clássico onde a ordem das vizinhanças é sorteada
     * aleatoriamente a cada reinício, evitando viés de ordenação fixo
     * e aumentando a diversidade da busca local.
     *
     * Diferença em relação ao VND:
     *   - VND  : percorre vizinhanças em ordem fixa predefinida
     *   - RVND : embaralha a lista de vizinhanças a cada reinício (k=0)
     */
    class RefinementHeuristicRVND {
    public:
        explicit RefinementHeuristicRVND(Instance& inst) : inst(inst) {}

        /**
         * Executa o RVND sobre a solução fornecida.
         * @param solution      Solução de entrada (modificada internamente)
         * @param neighborhoods Lista de vizinhanças disponíveis
         * @param localSearch   Estratégia de busca local (ex: BestImprovement)
         * @return              Melhor solução encontrada
         */
        Solution execute(Solution& solution,
                         std::vector<std::shared_ptr<INeighborhood>>& neighborhoods,
                         ILocalSearch& localSearch);

    private:
        Instance& inst;
    };

} // namespace opthirrygated

#endif // REFINEMENT_HEURISTIC_RVND_HPP