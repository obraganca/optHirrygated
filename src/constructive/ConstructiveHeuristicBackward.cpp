#include "constructive/ConstructiveHeuristicBackward.hpp"
#include "../include/Instance.hpp"
#include "../include/Solution.hpp"
#include <float.h>

#include <algorithm>
using namespace std;
using namespace opthirrygated;

Solution ConstructiveHeuristicBackward::execute() {Solution objSolution;
    vector<int> solution;
    vector<float> adfSol;

    float adi=0 , adf = 0;

    int numDays = inst.getCicle().size();
    for (int day = numDays-1; day >= 0; day--) {
        float bestScore = FLT_MAX;
        int bestPerc = -1;

        if(day == 0){
            adi = inst.getCad()[0];
        }else{
            adi = inst.getPrec()[day-1] - inst.getEtc()[day-1];
        }

        for (int perc : inst.getPerc()) {
            float auxAdf = adi - inst.getEtc()[day] + inst.getPrec()[day] + inst.getLamp()[perc];
            float percCost = inst.getCost()[perc];

            if (percCost < bestScore && auxAdf >= inst.getLc()[day]) {
                bestScore = percCost;
                bestPerc = perc;
                adf = auxAdf;
            }
        }

        if(bestPerc == -1){
            bestPerc = inst.getPerc()[inst.getPerc().size()-2];
            adf = adi - inst.getEtc()[day] + inst.getPrec()[day] + inst.getLamp()[bestPerc];
        }

        solution.push_back(bestPerc);
        adi = adf;
        adfSol.push_back(adf);
    }
    reverse(solution.begin(), solution.end());
    reverse(adfSol.begin(), adfSol.end());

    normalizerAdf(solution, adfSol);

    objSolution.setSolution(move(solution));
    objSolution.setAdfSolution(move(adfSol));
    return objSolution;
}

void ConstructiveHeuristicBackward::normalizerAdf(
        vector<int> &solution,
        vector<float> &solutionAdf
) {

    vector<int> auxSolution = solution;
    vector<float> auxSolutionAdf = solutionAdf;

    for(int day =0; day< solution.size(); day++){

        float adi = day == 0 ? inst.getCad()[0] : auxSolutionAdf[day-1];
        float auxAdf = adi - inst.getEtc()[day] + inst.getPrec()[day] + inst.getLamp()[auxSolution[day]];
        auxSolutionAdf[day] = auxAdf;
    }
    solution    = move(auxSolution);
    solutionAdf = move(auxSolutionAdf);
}