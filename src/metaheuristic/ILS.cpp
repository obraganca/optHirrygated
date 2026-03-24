#include "metaheuristic/LuusJaakola.hpp"
#include <random>
#include <algorithm>
#include <iostream>
#include "../include/Solution.hpp"
#include "../include/Measurer.hpp"

using namespace std;

namespace opthirrygated {

LuusJaakola::LuusJaakola(Instance& instance, int maxIt, float delta, float reduction)
    : inst(instance), maxIterations(maxIt), deltaInit(delta), rho(reduction) { }

Solution LuusJaakola::execute(const Solution& initialSolution) {
    if (initialSolution.getSolution().empty()) return initialSolution;

    Solution best = initialSolution;
    Measurer measurer(inst);

    int D = best.getSolution().size();
    static mt19937 rng(random_device{}());

    float delta = deltaInit;

    for (int iter = 0; iter < maxIterations; ++iter) {
        Solution candidate = best;

        // Escolhe um dia aleatório para perturbar
        uniform_int_distribution<int> dayDist(0, D - 1);
        int day = dayDist(rng);

        // Perturbação dentro de delta
        int perturb = uniform_int_distribution<int>(
            max(0, candidate.getSolution()[day] - int(delta)),
            min(int(inst.getLamp().size() - 1), candidate.getSolution()[day] + int(delta))
        )(rng);

        candidate.updateSolution(day, perturb);

        // Propaga mudanças e calcula limites críticos
        candidate.propagate(inst, day);
        candidate.constructCriticalLimitDelt(inst);

        // Avalia delta de custo
        float deltaCost = measurer.evaluate(candidate) - measurer.evaluate(best);

        // Verifica factibilidade
        if (measurer.isFeasible(candidate, day, 0.0f) && deltaCost < 0) {
            best = candidate;
        }

        delta *= rho; // reduz a região
        if (delta < 1.0f) delta = 1.0f; // mínimo de perturbação
    }

    return best;
}

} // namespace opthirrygated