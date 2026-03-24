#include "metaheuristic/ILS.hpp"
#include <algorithm>
#include <iostream>

using namespace opthirrygated;


// ------------------------------------------------------------
// Perturbação: altera `perturbStrength` dias aleatórios,
// propaga o balanço hídrico e recalcula o score completo.
// ------------------------------------------------------------
Solution ILS::perturb(const Solution& s) {
    const int   numDays  = (int)inst.getCicle().size();
    const auto& percs    = inst.getPerc();
    const int   numPercs = (int)percs.size();

    std::uniform_int_distribution<int> dayDist(0, numDays - 1);
    std::uniform_int_distribution<int> percDist(0, numPercs - 1);

    Solution perturbed  = s;
    int minChangedDay   = numDays;

    for (int i = 0; i < perturbStrength; ++i) {
        int day     = dayDist(rng);
        int newPerc = percs[percDist(rng)];

        perturbed.updateSolution(day, newPerc);
        minChangedDay = std::min(minChangedDay, day);
    }

    if (minChangedDay < numDays) {
        perturbed.propagate(inst, minChangedDay);
        perturbed.constructCriticalLimitDelt(inst);

        float score = 0.0f;
        for (int d = 0; d < numDays; ++d) {
            score += inst.getCost()[perturbed.getSolution()[d]];
        }
        perturbed.setScore(score);
    }

    return perturbed;
}

// ------------------------------------------------------------
// ILS principal
// ------------------------------------------------------------
Solution ILS::execute(Solution& solution) {
    if (solution.getSolution().empty()) return solution;

    const auto& lc = inst.getLc();

    auto isFullyFeasible = [&](const Solution& sol) -> bool {
        const auto& adf = sol.getAdfSolutions();
        for (int d = 0; d < (int)adf.size(); ++d) {
            if (adf[d] < lc[d]) return false;
        }
        return true;
    };

    // --------------------------------------------------------
    // Fase 0: RVND sobre a solução inicial
    // --------------------------------------------------------
    Solution current = refinement.execute(solution, neighborhoods, localSearch);
    Solution best    = current;

    std::cout << "[ILS] Score após refinement inicial: " << best.getScore() << "\n";

    int improvementCount = 0;
    int feasibleCount    = 0;

    // --------------------------------------------------------
    // Loop ILS
    // --------------------------------------------------------
    for (int iter = 0; iter < maxIterations; ++iter) {

        // a) Perturbação (diversificação)
        Solution perturbed = perturb(current);

        if (!isFullyFeasible(perturbed)) continue;
        ++feasibleCount;

        // b) RVND sobre perturbada (intensificação)
        Solution refined = refinement.execute(perturbed, neighborhoods, localSearch);

        // c) Aceitação gulosa
        if (refined.getScore() < current.getScore()) {
            current = refined;
            ++improvementCount;

            if (current.getScore() < best.getScore()) {
                best = current;
            }
        }
    }

    //std::cout << "[ILS] Iterações            : " << maxIterations    << "\n";
    //std::cout << "[ILS] Perturbações viáveis : " << feasibleCount    << "\n";
    //std::cout << "[ILS] Melhorias aceitas    : " << improvementCount << "\n";
    //std::cout << "[ILS] Score inicial        : " << solution.getScore() << "\n";
    //std::cout << "[ILS] Score final          : " << best.getScore()     << "\n";

    return best;
}