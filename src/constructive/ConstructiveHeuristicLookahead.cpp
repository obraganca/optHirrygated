#include "constructive/ConstructiveHeuristicLookahead.hpp"
#include "../include/Instance.hpp"
#include "../include/Solution.hpp"
#include <float.h>
#include <unordered_map>

using namespace std;
using namespace opthirrygated;

Solution ConstructiveHeuristicLookahead::execute() {
    Solution objSolution;
    vector<int> solution;
    vector<float> adfSol;

    float adi = inst.getCad()[0], adf = 0;
    float inf_f = std::numeric_limits<float>::infinity();
    size_t numDays = inst.getCicle().size();
    float bestTotalScore =0 ;
    for (size_t day = 0; day < numDays; day++) {
        int bestPerc = -1;
        float bestAdf = adi;
        float bestScore = FLT_MAX;

        for (int perc : inst.getPerc()) {
            float currAdf = adi - inst.getEtc()[day] + inst.getPrec()[day] + inst.getLamp()[perc];

            if (currAdf >= inst.getLc()[day]) {
                float currCost = inst.getCost()[perc];
                float futureCost = simulateLookahead(day + 1, currAdf, lookaheadDepth - 1);
                float totalScore = currCost + futureCost;

                if (totalScore < bestScore) {
                    bestScore = totalScore;
                    bestPerc = perc;
                    bestAdf = currAdf;
                }
            }
        }

        if (bestPerc != -1) {
            solution.push_back(bestPerc);
            bestTotalScore += inst.getCost()[bestPerc]; // <<< acumula aqui
            adf = bestAdf;
        } else {
            adf = adi; // fallback
            bestTotalScore+=inf_f;
        }

        adi = adf;
        adfSol.push_back(adf);
    }


    cout<<bestTotalScore<<endl;
    objSolution.setSolution(std::move(solution));
    objSolution.setAdfSolution(std::move(adfSol));
    objSolution.constructCriticalLimitDelt(inst);
    objSolution.setScore(bestTotalScore);
    return objSolution;
}



float ConstructiveHeuristicLookahead::simulateLookahead( size_t day, float adi, int depth) {
    if (day >= inst.getCicle().size() || depth == 0)
        return 0;

    float preAdf = adi - inst.getEtc()[day] + inst.getPrec()[day];

    float bestCost = FLT_MAX;

    if (preAdf >= inst.getCad()[day]) {
        bestCost = simulateLookahead(day + 1, preAdf, depth - 1);
    }

    for (int perc : inst.getPerc()) {
        float auxAdf = preAdf + inst.getLamp()[perc];
        float cost = inst.getCost()[perc];

        if (auxAdf >= inst.getLc()[day]) {
            float futureCost = simulateLookahead(day + 1, auxAdf, depth - 1);
            bestCost = std::min(bestCost, cost + futureCost);
        }
    }

    return bestCost;
}


