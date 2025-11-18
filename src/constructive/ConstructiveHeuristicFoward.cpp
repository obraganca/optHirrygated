#include "constructive/ConstructiveHeuristicFoward.hpp"
#include "../include/Instance.hpp"
#include "../include/Solution.hpp"
#include <float.h>

using namespace std;
using namespace opthirrygated;

Solution ConstructiveHeuristicFoward::execute() {
    Solution objSolution;
    vector<int> solution;
    vector<float> adfSol;

    float adi = inst.getCad()[0], adf = 0;
    size_t numDays = inst.getCicle().size();

    for (size_t day = 0; day < numDays; day++) {
        float bestPrice = FLT_MAX;
        int bestPerc = -1;

        for (int perc : inst.getPerc()) {
            float auxAdf = adi - inst.getEtc()[day] + inst.getPrec()[day] + inst.getLamp()[perc];
            float percCost = inst.getCost()[perc];

            if (percCost < bestPrice && auxAdf >= inst.getLc()[day]) {
                bestPrice = percCost;
                bestPerc = perc;
                adf = auxAdf;
            }
        }

        if (bestPerc != -1)
            solution.push_back(bestPerc);
        else
            adf = adi;

        adi = adf;
        adfSol.push_back(adf);
    }

    objSolution.setSolution(std::move(solution));
    objSolution.setAdfSolution(std::move(adfSol));
    return objSolution;
}
