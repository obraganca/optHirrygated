#include "constructive/ConstructiveHeuristicB.hpp"
#include "../include/Instance.hpp"
#include "../include/Solution.hpp"
#include <float.h>

using namespace std;
using namespace opthirrygated;


Solution ConstructiveHeuristicB::execute() {
    Solution objSolution;
    vector<int> solution;
    vector<float> adfSolution;

    float adi = inst.getCad()[0], adf = 0;

    size_t numDays = inst.getCicle().size();
    for (size_t day = 0; day < numDays; day++) {
        float bestPrice = FLT_MAX;
        int bestPerc = 10;

        float preAdf = adi - inst.getEtc()[day] + inst.getPrec()[day];

        if (preAdf >= inst.getCad()[day]) {
            solution.push_back(bestPerc);
            adi = preAdf;
            continue;
        }
        bestPerc=-1;
        float auxAdf =0;
        for (int perc : inst.getPerc()) {
            auxAdf = preAdf + inst.getLamp()[perc];
            float percCost = inst.getCost()[perc];

            if (percCost < bestPrice && auxAdf >= inst.getCad()[day]) {
                bestPrice = percCost;
                bestPerc = perc;
                adf = auxAdf;
            }
        }

        if(adf<inst.getLc()[day]){
            cout<<auxAdf << "ERROR"<<endl;
        }

        if (bestPerc != -1) {
            solution.push_back(bestPerc);
        } else {
            adf = adi;
        }

        adi = adf;
        adfSolution.push_back(adf);
    }

    objSolution.setSolution(std::move(solution));
    objSolution.setAdfSolution(std::move(adfSolution));
    return objSolution;
}