#include "constructive/ConstructiveHeuristicForwardA.hpp"
#include "../include/Instance.hpp"
#include "../include/Solution.hpp"
#include <float.h>

using namespace std;
using namespace opthirrygated;

Solution ConstructiveHeuristicForwardA::execute() {
    Solution objSolution;
    vector<int> solution;
    vector<float> adfSol;
    vector<float> criticalLimit;

    float adi = inst.getCad()[0], adf = 0;
    size_t numDays = inst.getCicle().size();
    float inf_f = std::numeric_limits<float>::infinity();

    float totalBestScore = 0;
    for (size_t day = 0; day < numDays; day++) {
        int bestPerc = -1;
        float bestPrice = FLT_MAX;
        for (int perc : inst.getPerc()) {
            float auxAdf = adi - inst.getEtc()[day] + inst.getPrec()[day] + inst.getLamp()[perc];
            float percCost = inst.getCost()[perc];

            if (percCost < bestPrice && auxAdf >= inst.getLc()[day]) {
                bestPrice = percCost;
                bestPerc = perc;
                adf = auxAdf;
            }
        }

        if(adf < inst.getLc()[day]){
            bestPrice = inf_f;
            adf = adi;
        }


        solution.push_back(bestPerc);
        adi = adf;
        adfSol.push_back(adf);
        totalBestScore+=bestPrice;
    }


    objSolution.setScore(totalBestScore);
    objSolution.setSolution(std::move(solution));
    objSolution.setAdfSolution(std::move(adfSol));
    objSolution.constructCriticalLimitDelt(inst);
    return objSolution;
}


