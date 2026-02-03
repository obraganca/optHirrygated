#include "constructive/ConstructiveHeuristicC.hpp"
#include "../include/Instance.hpp"
#include "../include/Solution.hpp"
#include <float.h>

using namespace std;
using namespace opthirrygated;

Solution ConstructiveHeuristicC::execute() {
    Solution objSolution;
    vector<int> solution;
    vector<float> adfSol;

    float inf_f = std::numeric_limits<float>::infinity();
    float adi = inst.getCad()[0], adf = 0;

    size_t numDays = inst.getCicle().size();
    for (size_t day = 0; day < numDays; day++) {
        float bestPrice = FLT_MAX;
        int bestPerc = 10;

        float preAdf = adi - inst.getEtc()[day] + inst.getPrec()[day];

        bestPerc=-1;
        for (int perc : inst.getPerc()) {
            float auxAdf = preAdf + inst.getLamp()[perc];
            float percCost = inst.getCost()[perc];

            if (auxAdf <= inst.getCad()[day] && auxAdf >= inst.getLc()[day]) {
                bestPrice = percCost;
                bestPerc = perc;
                adf = auxAdf;
            }
        }

        if(adf<inst.getLc()[day]){
            bestPrice = inf_f;
            adf = adi;
        }


        solution.push_back(bestPerc);
        adi = adf;
        adfSol.push_back(adf);
    }

    objSolution.setSolution(std::move(solution));
    objSolution.setAdfSolution(std::move(adfSol));
    return objSolution;
}
